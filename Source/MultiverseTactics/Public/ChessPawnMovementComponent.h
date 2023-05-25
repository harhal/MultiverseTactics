// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridPlane.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ChessPawnMovementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIVERSETACTICS_API UChessPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UChessPawnMovementComponent();
	
	UFUNCTION(BlueprintCallable)
	void PutOnBoard(UGridPlane* Grid, const FGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void TeleportToCell(const FGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void MoveToCell(const FGridCell& Cell);

	UFUNCTION(BlueprintPure)
	FGridCell GetCurrentCell() const;

	UFUNCTION(BlueprintPure)
	UGridPlane* GetCurrentGrid() const;

	UFUNCTION(BlueprintPure)
	void GetCurrentWay(TArray<FGridCell>& OutCurrentWay) const;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TWeakObjectPtr<UGridPlane> CurrentGrid;

	UPROPERTY()
	FGridCell CurrentCell;

	UPROPERTY()
	TArray<FGridCell> CurrentWay;

	//Cells per second
	UPROPERTY(EditDefaultsOnly)
	float Speed = 0.5f;

	void MoveToNextCell();
};
