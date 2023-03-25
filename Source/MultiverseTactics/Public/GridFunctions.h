// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCell.h"
#include "GridCellOffset.h"
#include "GridFunctions.generated.h"

/**
 * 
 */
UCLASS()
class MULTIVERSETACTICS_API UGridFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Horizontal"))
	static FGridCellOffset MakeHorizontalGridCellOffset();

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "UpDiagonal"))
	static FGridCellOffset MakeUprisingDiagonalGridCellOffset();

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "DownDiagonal"))
	static FGridCellOffset MakeFallingDiagonalGridCellOffset();

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Down"))
	static FGridCellOffset MakeDefaultDownCellOffset(bool bEven, int Length);

	UFUNCTION(BlueprintPure, DisplayName = "Add",
		meta = (CompactNodeTitle = "+", CommutativeAssociativeBinaryOperator = true))
	static FGridCellOffset AddGridCellOffset(const FGridCellOffset& A, const FGridCellOffset& B);

	UFUNCTION(BlueprintPure, DisplayName = "Multiply",
		meta = (CompactNodeTitle = "*"))
	static FGridCellOffset MultiplyGridCellOffset(const FGridCellOffset& A, float B);

	UFUNCTION(BlueprintPure, DisplayName = "Optimize")
	static FGridCellOffset OptimizeToShortestWay(const FGridCellOffset& A);

	UFUNCTION(BlueprintPure, DisplayName = "Length")
	static int GetGridCellOffsetLength(const FGridCellOffset& Offset);

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "+"))
	static FGridCell AddOffsetToCell(const FGridCell& Cell, const FGridCellOffset& Offset);

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "-"))
	static FGridCellOffset GetOffsetBetweenCells(const FGridCell& A, const FGridCell& B);

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "BuildWay"))
	static void BuildStraightWay(const FGridCell& Cell, const FGridCellOffset& Offset, TArray<FGridCell>& OutWay);
};
