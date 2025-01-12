// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridPlane.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChessPawnMovementComponent.generated.h"

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
		FGridCell PreviousCell;
		FGridCell TargetCell;
		FTimerHandle TransitionTimerHandle;
	};
	
	UChessPawnMovementComponent();
	
	UFUNCTION(BlueprintCallable)
	void PutOnBoard(UGridPlane* Grid, const FGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void TeleportToCell(const FGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void MoveToCell(const FGridCell& Cell);

	UFUNCTION(BlueprintCallable)
	void SkipTransition();

	UFUNCTION(BlueprintPure)
	FGridCell GetCurrentCell() const;

	UFUNCTION(BlueprintPure)
	UGridPlane* GetCurrentGrid() const;

	UFUNCTION(BlueprintPure)
	void GetCurrentStepsStack(TArray<FGridCell>& OutStepsStack) const;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual bool IsFalling() const override;

	UFUNCTION(BlueprintPure)
	EChessPawnVisualState GetVisualState() const;

	UFUNCTION(BlueprintCallable)
	bool GetTransitionInfo(FGridCell& OutPreviousCell, FGridCell& OutTargetCell) const;

private:
	UPROPERTY()
	TWeakObjectPtr<UGridPlane> CurrentGrid = nullptr;

	UPROPERTY()
	FGridCell CurrentCell = FGridCell();

	UPROPERTY()
	TArray<FGridCell> StepsStack = TArray<FGridCell>();

	//Cells per second
	UPROPERTY(EditDefaultsOnly)
	float Speed = 0.5f;

	UPROPERTY()
	EChessPawnVisualState VisualState = EChessPawnVisualState::Idle;

	FTransitionInfo TransitionInfo = FTransitionInfo();

	void MoveToNextCell();
};
