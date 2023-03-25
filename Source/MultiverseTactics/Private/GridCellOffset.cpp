// Fill out your copyright notice in the Description page of Project Settings.


#include "GridCellOffset.h"

const FGridCellOffset FGridCellOffset::OneHorizontalOffset =
	FGridCellOffset{1, 0, 0};
const FGridCellOffset FGridCellOffset::OneUprisingDiagonalOffset =
	FGridCellOffset{0, 1, 0};
const FGridCellOffset FGridCellOffset::OneFallingDiagonalOffset =
	FGridCellOffset{0, 0, 1};

FGridCellOffset FGridCellOffset::operator+(const FGridCellOffset& OtherOffset) const
{
	return FGridCellOffset{
		HorizontalOffset + OtherOffset.HorizontalOffset,
		UprisingDiagonalOffset + OtherOffset.UprisingDiagonalOffset,
		FallingDiagonalOffset + OtherOffset.FallingDiagonalOffset
	};
}

FGridCellOffset FGridCellOffset::operator-(const FGridCellOffset& OtherOffset) const
{
	return FGridCellOffset{
		HorizontalOffset - OtherOffset.HorizontalOffset,
		UprisingDiagonalOffset - OtherOffset.UprisingDiagonalOffset,
		FallingDiagonalOffset - OtherOffset.FallingDiagonalOffset
	};
}

const FGridCellOffset& FGridCellOffset::operator+=(const FGridCellOffset& OtherOffset)
{
	HorizontalOffset += OtherOffset.HorizontalOffset;
	UprisingDiagonalOffset += OtherOffset.UprisingDiagonalOffset;
	FallingDiagonalOffset += OtherOffset.FallingDiagonalOffset;

	return *this;
}

const FGridCellOffset& FGridCellOffset::operator-=(const FGridCellOffset& OtherOffset)
{
	HorizontalOffset -= OtherOffset.HorizontalOffset;
	UprisingDiagonalOffset -= OtherOffset.UprisingDiagonalOffset;
	FallingDiagonalOffset -= OtherOffset.FallingDiagonalOffset;

	return *this;
}

FGridCellOffset FGridCellOffset::operator*(int A) const
{
	return FGridCellOffset{
		HorizontalOffset * A,
		UprisingDiagonalOffset * A,
		FallingDiagonalOffset * A
	};
}

FGridCellOffset FGridCellOffset::operator/(int A) const
{
	return FGridCellOffset{
		HorizontalOffset / A,
		UprisingDiagonalOffset / A,
		FallingDiagonalOffset / A
	};
}

const FGridCellOffset& FGridCellOffset::operator*=(int A)
{
	HorizontalOffset *= A;
	UprisingDiagonalOffset *= A;
	FallingDiagonalOffset *= A;

	return *this;
}

const FGridCellOffset& FGridCellOffset::operator/=(int A)
{
	HorizontalOffset /= A;
	UprisingDiagonalOffset /= A;
	FallingDiagonalOffset /= A;

	return *this;
}

void FGridCellOffset::OptimizeToShortestWay()
{
	int Trinity[3] = {-HorizontalOffset, UprisingDiagonalOffset, FallingDiagonalOffset};
	Algo::Sort(Trinity);
	const int Correction = Trinity[1];

	HorizontalOffset = HorizontalOffset + Correction;
	UprisingDiagonalOffset = UprisingDiagonalOffset - Correction;
	FallingDiagonalOffset = FallingDiagonalOffset - Correction;
}
