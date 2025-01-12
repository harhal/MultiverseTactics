// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHGridCell.h"
#include "HHGridCellOffset.h"
#include "HHGridFunctions.generated.h"

/**
 * 
 */
UCLASS()
class HARHALHEXAGONALGRID_API UHhGridFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Horizontal"))
	static FHhGridCellOffset MakeHorizontalGridCellOffset();

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "UpDiagonal"))
	static FHhGridCellOffset MakeUprisingDiagonalGridCellOffset();

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "DownDiagonal"))
	static FHhGridCellOffset MakeFallingDiagonalGridCellOffset();

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Down"))
	static FHhGridCellOffset MakeDefaultDownCellOffset(bool bEven, int Length);

	UFUNCTION(BlueprintPure, DisplayName = "Add",
		meta = (CompactNodeTitle = "+", CommutativeAssociativeBinaryOperator = true))
	static FHhGridCellOffset AddGridCellOffset(const FHhGridCellOffset& A, const FHhGridCellOffset& B);

	UFUNCTION(BlueprintPure, DisplayName = "Multiply",
		meta = (CompactNodeTitle = "*"))
	static FHhGridCellOffset MultiplyGridCellOffset(const FHhGridCellOffset& A, float B);

	UFUNCTION(BlueprintPure, DisplayName = "Optimize")
	static FHhGridCellOffset OptimizeToShortestWay(const FHhGridCellOffset& A);

	UFUNCTION(BlueprintPure, DisplayName = "Length")
	static int GetGridCellOffsetLength(const FHhGridCellOffset& Offset);

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "+"))
	static FHhGridCell AddOffsetToCell(const FHhGridCell& Cell, const FHhGridCellOffset& Offset);

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "-"))
	static FHhGridCellOffset GetOffsetBetweenCells(const FHhGridCell& A, const FHhGridCell& B);

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "BuildWay"))
	static void BuildStraightWay(const FHhGridCell& Cell, const FHhGridCellOffset& Offset, TArray<FHhGridCell>& OutWay);
};
