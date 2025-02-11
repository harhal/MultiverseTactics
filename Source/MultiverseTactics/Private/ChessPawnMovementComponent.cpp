﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPawnMovementComponent.h"

#include "HHGridFunctions.h"
#include "HHGridPlane.h"
#include "GameFramework/RootMotionSource.h"

UChessPawnMovementComponent::UChessPawnMovementComponent()
{
}

void UChessPawnMovementComponent::PutOnBoard(UHhGridPlane* Grid, const FHhGridCell& Cell)
{
	GetOwner()->SetActorEnableCollision(false);
	SetMovementMode(MOVE_Custom);
	CurrentGrid = Grid;
	TeleportToCell(Cell);
}

void UChessPawnMovementComponent::TeleportToCell(const FHhGridCell& Cell)
{
	const FVector& DestLocation = GetPawnLocationForGridCell(Cell);
	const FRotator& DestRotation = GetPawnOwner()->GetActorRotation();
	GetPawnOwner()->TeleportTo(DestLocation, DestRotation);
	
	CurrentCell = Cell;
}

void UChessPawnMovementComponent::MoveToCell(const FHhGridCell& Cell)
{
	const FHhGridCellOffset& Offset = UHhGridFunctions::GetOffsetBetweenCells(CurrentCell, Cell);

	TArray<FHhGridCell> Way;
	UHhGridFunctions::BuildStraightWay(CurrentCell, Offset, Way);

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
	const FHhGridCell NextCell = StepsStack.Pop();
	
	const TSharedPtr<FRootMotionSource_MoveToForce>& RootMotionSource = MakeShared<FRootMotionSource_MoveToForce>();
	RootMotionSource->TargetLocation = GetPawnLocationForGridCell(NextCell);
	RootMotionSource->StartLocation = GetPawnLocationForGridCell(CurrentCell);
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

FVector UChessPawnMovementComponent::GetPawnLocationForGridCell(const FHhGridCell& Cell) const
{
	const float HalfHeight = GetPawnCapsuleExtent(SHRINK_None).Z;
	const FVector& Offset = GetOwner()->GetActorRotation().RotateVector(FVector::DownVector) * HalfHeight;
	return CurrentGrid->GetCellCenterLocation(Cell) - Offset;
}

void UChessPawnMovementComponent::SkipTransition()
{
	if (VisualState != EChessPawnVisualState::InTransition)
	{
		return;
	}

	if (!StepsStack.IsEmpty())
	{
		const FHhGridCell LastCell = StepsStack[0];
		CurrentCell = LastCell;
		StepsStack.Empty();
	}
	
	const FVector& DestLocation = GetPawnLocationForGridCell(CurrentCell);
	const FRotator& DestRotation = GetPawnOwner()->GetActorRotation();
	GetPawnOwner()->TeleportTo(DestLocation, DestRotation);

	GetWorld()->GetTimerManager().ClearTimer(TransitionInfo.TransitionTimerHandle);
	RemoveRootMotionSource(RootMotionSourceName);
	
	VisualState = EChessPawnVisualState::Idle;
	Velocity = FVector::Zero();
}

FHhGridCell UChessPawnMovementComponent::GetCurrentCell() const
{
	return CurrentCell;
}

UHhGridPlane* UChessPawnMovementComponent::GetCurrentGrid() const
{
	return CurrentGrid.Get();
}

void UChessPawnMovementComponent::GetCurrentStepsStack(TArray<FHhGridCell>& OutCurrentWay) const
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

	const FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * deltaTime;
	const FQuat Rotation = Velocity.IsNearlyZero() ? UpdatedComponent->GetComponentQuat() : Velocity.ToOrientationRotator().Quaternion();
	FHitResult Hit{};
	SafeMoveUpdatedComponent(Adjusted, Rotation, true, Hit);

	Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;

	PhysicsRotation(deltaTime);
}

bool UChessPawnMovementComponent::IsFalling() const
{
	return Super::IsFalling();
}

EChessPawnVisualState UChessPawnMovementComponent::GetVisualState() const
{
	return VisualState;
}

bool UChessPawnMovementComponent::GetTransitionInfo(FHhGridCell& OutPreviousCell, FHhGridCell& OutTargetCell) const
{
	if (VisualState != EChessPawnVisualState::InTransition)
	{
		return false;
	}

	OutPreviousCell = TransitionInfo.PreviousCell;
	OutTargetCell = TransitionInfo.TargetCell;
	return true;
}
