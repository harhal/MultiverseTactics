#include "ChessPawn.h"

#include "ChessPawnMovementComponent.h"
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

void AChessPawn::PutPawnOnBoard(UHhGridPlane* Grid, const FHhGridCell& Cell)
{
	GetChessPawnMovementComponent()->PutOnBoard(Grid, Cell);
}

void AChessPawn::TeleportToCell(const FHhGridCell& Cell)
{
	GetChessPawnMovementComponent()->TeleportToCell(Cell);
}

void AChessPawn::MoveToCell(const FHhGridCell& Cell)
{
	GetChessPawnMovementComponent()->MoveToCell(Cell);
}

TObjectPtr<UChessPawnMovementComponent> AChessPawn::GetChessPawnMovementComponent() const
{
	return Cast<UChessPawnMovementComponent>(GetMovementComponent());
}
