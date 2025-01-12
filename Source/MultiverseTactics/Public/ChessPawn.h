// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHGridCell.h"
#include "GameFramework/Character.h"
#include "ChessPawn.generated.h"

class UHhGridPlane;

UCLASS()
class MULTIVERSETACTICS_API AChessPawn : public ACharacter
{
	GENERATED_BODY()

public:
	AChessPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void PutPawnOnBoard(UHhGridPlane* Grid, const FHhGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void TeleportToCell(const FHhGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void MoveToCell(const FHhGridCell& Cell);

	TObjectPtr<class UChessPawnMovementComponent> GetChessPawnMovementComponent() const;
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TWeakObjectPtr<UHhGridPlane> DefaultGrid;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FHhGridCell DefaultGridCell;
};
