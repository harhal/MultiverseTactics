// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHGridCell.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChessPawnMovementComponent.generated.h"

class UHhGridPlane;

UENUM(Blueprintable)
enum class EChessPawnVisualState : uint8
{
	Idle,
	InTransition
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIVERSETACTICS_API UChessPawnMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	struct FTransitionInfo
	{
		FHhGridCell PreviousCell;
		FHhGridCell TargetCell;
		FTimerHandle TransitionTimerHandle;
	};
	
	UChessPawnMovementComponent();
	
	UFUNCTION(BlueprintCallable)
	void PutOnBoard(UHhGridPlane* Grid, const FHhGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void TeleportToCell(const FHhGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void MoveToCell(const FHhGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void SkipTransition();

	UFUNCTION(BlueprintPure)
	FHhGridCell GetCurrentCell() const;

	UFUNCTION(BlueprintPure)
	UHhGridPlane* GetCurrentGrid() const;

	UFUNCTION(BlueprintPure)
	void GetCurrentStepsStack(TArray<FHhGridCell>& OutStepsStack) const;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual bool IsFalling() const override;

	UFUNCTION(BlueprintPure)
	EChessPawnVisualState GetVisualState() const;

	UFUNCTION(BlueprintCallable)
	bool GetTransitionInfo(FHhGridCell& OutPreviousCell, FHhGridCell& OutTargetCell) const;

private:
	UPROPERTY()
	TWeakObjectPtr<UHhGridPlane> CurrentGrid = nullptr;

	UPROPERTY()
	FHhGridCell CurrentCell = FHhGridCell();

	UPROPERTY()
	TArray<FHhGridCell> StepsStack = TArray<FHhGridCell>();

	//Cells per second
	UPROPERTY(EditDefaultsOnly)
	float Speed = 0.5f;

	UPROPERTY()
	EChessPawnVisualState VisualState = EChessPawnVisualState::Idle;

	FTransitionInfo TransitionInfo = FTransitionInfo();

	void MoveToNextCell();

	FVector GetPawnLocationForGridCell(const FHhGridCell& Cell) const;
};
