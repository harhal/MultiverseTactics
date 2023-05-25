// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCell.h"
#include "GridPlane.h"
#include "GameFramework/Character.h"
#include "ChessPawn.generated.h"

UCLASS()
class MULTIVERSETACTICS_API AChessPawn : public ACharacter
{
	GENERATED_BODY()

public:
	AChessPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void PutPawnOnBoard(UGridPlane* Grid, const FGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void TeleportToCell(const FGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void MoveToCell(const FGridCell& Cell);

	TObjectPtr<class UChessPawnMovementComponent> GetChessPawnMovementComponent() const;
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TWeakObjectPtr<UGridPlane> DefaultGrid;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FGridCell DefaultGridCell;
};
