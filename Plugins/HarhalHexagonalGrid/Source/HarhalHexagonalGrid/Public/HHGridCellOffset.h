// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHGridCellOffset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct HARHALHEXAGONALGRID_API FHhGridCellOffset
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int HorizontalOffset = 0;

	//Diagonal from Bottom Left to Top Right, '/'
	UPROPERTY(BlueprintReadWrite)
	int UprisingDiagonalOffset = 0;

	//Diagonal from Top Left to Bottom Right '\'
	UPROPERTY(BlueprintReadWrite)
	int FallingDiagonalOffset = 0;
	
	static const FHhGridCellOffset OneHorizontalOffset;
	static const FHhGridCellOffset OneUprisingDiagonalOffset;
	static const FHhGridCellOffset OneFallingDiagonalOffset;

	FHhGridCellOffset operator+(const FHhGridCellOffset& OtherOffset) const;
	FHhGridCellOffset operator-(const FHhGridCellOffset& OtherOffset) const;

	const FHhGridCellOffset& operator+=(const FHhGridCellOffset& OtherOffset);
	const FHhGridCellOffset& operator-=(const FHhGridCellOffset& OtherOffset);

	FHhGridCellOffset operator*(int A) const;
	FHhGridCellOffset operator/(int A) const;

	const FHhGridCellOffset& operator*=(int A);
	const FHhGridCellOffset& operator/=(int A);

	int GetLength() const;

	void OptimizeToShortestWay();
};
