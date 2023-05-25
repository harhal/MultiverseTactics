// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPawnMovementComponent.h"

#include "GridCellOffset.h"
#include "GridFunctions.h"

UChessPawnMovementComponent::UChessPawnMovementComponent()
{
}

void UChessPawnMovementComponent::PutOnBoard(UGridPlane* Grid, const FGridCell& Cell)
{
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
	const FGridCellOffset& InversedWay = UGridFunctions::GetOffsetBetweenCells(Cell, CurrentCell);

	UGridFunctions::BuildStraightWay(Cell, InversedWay, CurrentWay);

	MoveToNextCell();	
}

void UChessPawnMovementComponent::MoveToNextCell()
{
	if (CurrentWay.Num() <= 0)
	{
		return;
	}
	
	const float StepDuration = 1.f / Speed;
	const FGridCell& NextCell = CurrentWay[CurrentWay.Num() - 1];

	
	
	const TSharedPtr<FRootMotionSource_MoveToForce>& RootMotionSource = MakeShared<FRootMotionSource_MoveToForce>();
	RootMotionSource->TargetLocation = CurrentGrid->GetCellCenterLocation(NextCell);
	RootMotionSource->StartLocation = CurrentGrid->GetCellCenterLocation(CurrentCell);
	RootMotionSource->Duration = StepDuration;

	ApplyRootMotionSource(RootMotionSource);

	FTimerHandle DumpHandle;
	GetWorld()->GetTimerManager().SetTimer(DumpHandle, [NextCell, this]()
	{
		CurrentCell = NextCell;

		MoveToNextCell();
	}, StepDuration, false);
}

FGridCell UChessPawnMovementComponent::GetCurrentCell() const
{
	return CurrentCell;
}

UGridPlane* UChessPawnMovementComponent::GetCurrentGrid() const
{
	return CurrentGrid.Get();
}

void UChessPawnMovementComponent::GetCurrentWay(TArray<FGridCell>& OutCurrentWay) const
{
	OutCurrentWay = CurrentWay;
}

void UChessPawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveUpdatedComponent()
}