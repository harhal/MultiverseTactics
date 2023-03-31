// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCell.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MULTIVERSETACTICS_API FGridCell
{
	GENERATED_BODY()

	FGridCell() = default;

	FGridCell(int Column, int Line) : CellColumn(Column), CellLine(Line)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CellColumn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CellLine;
	
	int GetLine() const
	{
		return CellLine;
	}
	
	int GetColumn() const
	{
		return CellColumn;
	}
	
	void SetLine(int Line)
	{
		CellLine = Line;
	}
	
	void SetColumn(int Column)
	{
		CellColumn = Column;
	}

	friend uint32 GetTypeHash(FGridCell GridCell)
	{
		return GridCell.CellColumn + (GridCell.CellLine >> 16);
	}
};
