#include "ChessPawn.h"

#include "ChessPawnMovementComponent.h"
#include "GridFunctions.h"
#include "GridPlane.h"
#include "GameFramework/Character.h"


AChessPawn::AChessPawn(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UChessPawnMovementComponent>(
		CharacterMovementComponentName)), DefaultGridCell()
{
}

void AChessPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!DefaultGrid.IsValid())
	{
		return;
	}

	PutPawnOnBoard(DefaultGrid.Get(), DefaultGridCell);
}

void AChessPawn::PutPawnOnBoard(UGridPlane* Grid, const FGridCell& Cell)
{
	GetChessPawnMovementComponent()->PutOnBoard(Grid, Cell);
}

void AChessPawn::TeleportToCell(const FGridCell& Cell)
{
	GetChessPawnMovementComponent()->TeleportToCell(Cell);
}

void AChessPawn::MoveToCell(const FGridCell& Cell)
{
	GetChessPawnMovementComponent()->MoveToCell(Cell);
}

TObjectPtr<UChessPawnMovementComponent> AChessPawn::GetChessPawnMovementComponent() const
{
	return Cast<UChessPawnMovementComponent>(GetMovementComponent());
}
