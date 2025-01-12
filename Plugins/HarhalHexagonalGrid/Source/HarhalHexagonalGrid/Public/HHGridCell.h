// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHGridCell.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct HARHALHEXAGONALGRID_API FHhGridCell
{
	GENERATED_BODY()

	FHhGridCell() = default;

	FHhGridCell(const int Column, const int Line);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CellColumn = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CellLine = 0;
	
	int GetLine() const;
	int GetColumn() const;
	void SetLine(const int Line);
	void SetColumn(const int Column);
	friend uint32 GetTypeHash(const FHhGridCell GridCell);
};
