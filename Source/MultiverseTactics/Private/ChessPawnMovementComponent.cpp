// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPawnMovementComponent.h"

#include "GridCellOffset.h"
#include "GridFunctions.h"
#include "GameFramework/RootMotionSource.h"

UChessPawnMovementComponent::UChessPawnMovementComponent()
{
}

void UChessPawnMovementComponent::PutOnBoard(UGridPlane* Grid, const FGridCell& Cell)
{
	GetOwner()->SetActorEnableCollision(false);
	SetMovementMode(MOVE_Custom);
	CurrentGrid = Grid;
	TeleportToCell(Cell);
}

void UChessPawnMovementComponent::TeleportToCell(const FGridCell& Cell)
{
	const FVector& DestLocation = CurrentGrid->GetCellCenterLocation(Cell);
	const FRotator& DestRotation = GetPawnOwner()->GetActorRotation();
	GetPawnOwner()->TeleportTo(DestLocation, DestRotation);
	
	CurrentCell = Cell;
}

void UChessPawnMovementComponent::MoveToCell(const FGridCell& Cell)
{
	const FGridCellOffset& Offset = UGridFunctions::GetOffsetBetweenCells(CurrentCell, Cell);

	TArray<FGridCell> Way;
	UGridFunctions::BuildStraightWay(CurrentCell, Offset, Way);

	int32 StepsCount = Way.Num() - 1;
	StepsStack.Reserve(StepsCount);

	for (int idx = 0; idx < StepsCount; idx++)
	{
		StepsStack.Push(Way.Pop(false));
	}

	MoveToNextCell();	
}

const FName RootMotionSourceName = TEXT("MoveToNextCell");

void UChessPawnMovementComponent::MoveToNextCell()
{
	if (StepsStack.Num() <= 0)
	{
		return;
	}
	
	const float StepDuration = 1.f / Speed;
	const FGridCell NextCell = StepsStack.Pop();
	
	const TSharedPtr<FRootMotionSource_MoveToForce>& RootMotionSource = MakeShared<FRootMotionSource_MoveToForce>();
	RootMotionSource->TargetLocation = CurrentGrid->GetCellCenterLocation(NextCell);
	RootMotionSource->StartLocation = CurrentGrid->GetCellCenterLocation(CurrentCell);
	RootMotionSource->Duration = StepDuration;
	RootMotionSource->InstanceName = RootMotionSourceName;
	
	TransitionInfo.PreviousCell = CurrentCell;
	TransitionInfo.TargetCell = NextCell;
	
	ApplyRootMotionSource(RootMotionSource);
	CurrentCell = NextCell;
	VisualState = EChessPawnVisualState::InTransition;

	GetWorld()->GetTimerManager().SetTimer(TransitionInfo.TransitionTimerHandle, [this]()
	{
		RemoveRootMotionSource(RootMotionSourceName);

		if (!StepsStack.IsEmpty())
		{
			MoveToNextCell();
		}
		else
		{			
			VisualState = EChessPawnVisualState::Idle;
			Velocity = FVector::Zero();
		}
	}, StepDuration, false);
}

void UChessPawnMovementComponent::SkipTransition()
{
	if (VisualState != EChessPawnVisualState::InTransition)
	{
		return;
	}

	if (!StepsStack.IsEmpty())
	{
		const FGridCell LastCell = StepsStack[0];
		CurrentCell = LastCell;
		StepsStack.Empty();
	}
	
	const FVector& DestLocation = CurrentGrid->GetCellCenterLocation(CurrentCell);
	const FRotator& DestRotation = GetPawnOwner()->GetActorRotation();
	GetPawnOwner()->TeleportTo(DestLocation, DestRotation);

	GetWorld()->GetTimerManager().ClearTimer(TransitionInfo.TransitionTimerHandle);
	RemoveRootMotionSource(RootMotionSourceName);
	
	VisualState = EChessPawnVisualState::Idle;
	Velocity = FVector::Zero();
}

FGridCell UChessPawnMovementComponent::GetCurrentCell() const
{
	return CurrentCell;
}

UGridPlane* UChessPawnMovementComponent::GetCurrentGrid() const
{
	return CurrentGrid.Get();
}

void UChessPawnMovementComponent::GetCurrentStepsStack(TArray<FGridCell>& OutCurrentWay) const
{
	OutCurrentWay = StepsStack;
}

void UChessPawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UChessPawnMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}
	
	Super::PhysCustom(deltaTime, Iterations);
	
	ApplyRootMotionToVelocity(deltaTime);

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

	Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
}

bool UChessPawnMovementComponent::IsFalling() const
{
	return Super::IsFalling();
}

EChessPawnVisualState UChessPawnMovementComponent::GetVisualState() const
{
	return VisualState;
}

bool UChessPawnMovementComponent::GetTransitionInfo(FGridCell& OutPreviousCell, FGridCell& OutTargetCell) const
{
	if (VisualState != EChessPawnVisualState::InTransition)
	{
		return false;
	}

	OutPreviousCell = TransitionInfo.PreviousCell;
	OutTargetCell = TransitionInfo.TargetCell;
	return true;
}
