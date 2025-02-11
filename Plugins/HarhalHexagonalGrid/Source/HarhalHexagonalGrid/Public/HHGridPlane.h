﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HHGridCell.h"
#include "HHGridPlane.generated.h"

/**
 * 
 */
 
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent = true))
class HARHALHEXAGONALGRID_API UHhGridPlane : public USceneComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FHhGridCell GetCellByPoint(FVector Point) const;
	
	UFUNCTION(BlueprintCallable)
	FVector GetCellCenterLocation(const FHhGridCell& Cell) const;
	
	UFUNCTION(BlueprintPure)
	bool IsValidCell(const FHhGridCell& Cell) const;

	FVector LocalToWorldPoint(const FVector& Point) const;

	FVector WorldToLocalPoint(const FVector& Point) const;
	
	static FHhGridCell GetCellByLocalPoint(const FVector& Point);
	
	static FVector GetCellCenterLocalLocation(const FHhGridCell& Cell);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CellSize = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FIntVector GridSize = FIntVector(1, 1, 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bAutoCenter;

private:
	static constexpr float LocalCellLongestRadius = 0.5f;
	static constexpr float CellFullAngle = 2.f*PI/*2pi radians or 360 degrees*/;
	static constexpr float AngleBetweenLongestAndShortestRadius = CellFullAngle/12.f;
	
	static float GetCellsLocalHorizontalHalfOffset();

	static float GetCellsLocalVerticalOffset();

	FVector GetLocalCenterOffset() const;
};