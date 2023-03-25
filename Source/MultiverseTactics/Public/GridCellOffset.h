// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCellOffset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MULTIVERSETACTICS_API FGridCellOffset
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
	
	static const FGridCellOffset OneHorizontalOffset;
	static const FGridCellOffset OneUprisingDiagonalOffset;
	static const FGridCellOffset OneFallingDiagonalOffset;

	FGridCellOffset operator+(const FGridCellOffset& OtherOffset) const;
	FGridCellOffset operator-(const FGridCellOffset& OtherOffset) const;

	const FGridCellOffset& operator+=(const FGridCellOffset& OtherOffset);
	const FGridCellOffset& operator-=(const FGridCellOffset& OtherOffset);

	FGridCellOffset operator*(int A) const;
	FGridCellOffset operator/(int A) const;

	const FGridCellOffset& operator*=(int A);
	const FGridCellOffset& operator/=(int A);

	int GetLength() const;

	void OptimizeToShortestWay();
};
