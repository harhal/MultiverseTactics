// Fill out your copyright notice in the Description page of Project Settings.


#include "HHGridCellOffset.h"

const FHhGridCellOffset FHhGridCellOffset::OneHorizontalOffset =
	FHhGridCellOffset{1, 0, 0};
const FHhGridCellOffset FHhGridCellOffset::OneUprisingDiagonalOffset =
	FHhGridCellOffset{0, 1, 0};
const FHhGridCellOffset FHhGridCellOffset::OneFallingDiagonalOffset =
	FHhGridCellOffset{0, 0, 1};

FHhGridCellOffset FHhGridCellOffset::operator+(const FHhGridCellOffset& OtherOffset) const
{
	return FHhGridCellOffset{
		HorizontalOffset + OtherOffset.HorizontalOffset,
		UprisingDiagonalOffset + OtherOffset.UprisingDiagonalOffset,
		FallingDiagonalOffset + OtherOffset.FallingDiagonalOffset
	};
}

FHhGridCellOffset FHhGridCellOffset::operator-(const FHhGridCellOffset& OtherOffset) const
{
	return FHhGridCellOffset{
		HorizontalOffset - OtherOffset.HorizontalOffset,
		UprisingDiagonalOffset - OtherOffset.UprisingDiagonalOffset,
		FallingDiagonalOffset - OtherOffset.FallingDiagonalOffset
	};
}

const FHhGridCellOffset& FHhGridCellOffset::operator+=(const FHhGridCellOffset& OtherOffset)
{
	HorizontalOffset += OtherOffset.HorizontalOffset;
	UprisingDiagonalOffset += OtherOffset.UprisingDiagonalOffset;
	FallingDiagonalOffset += OtherOffset.FallingDiagonalOffset;

	return *this;
}

const FHhGridCellOffset& FHhGridCellOffset::operator-=(const FHhGridCellOffset& OtherOffset)
{
	HorizontalOffset -= OtherOffset.HorizontalOffset;
	UprisingDiagonalOffset -= OtherOffset.UprisingDiagonalOffset;
	FallingDiagonalOffset -= OtherOffset.FallingDiagonalOffset;

	return *this;
}

FHhGridCellOffset FHhGridCellOffset::operator*(int A) const
{
	return FHhGridCellOffset{
		HorizontalOffset * A,
		UprisingDiagonalOffset * A,
		FallingDiagonalOffset * A
	};
}

FHhGridCellOffset FHhGridCellOffset::operator/(int A) const
{
	return FHhGridCellOffset{
		HorizontalOffset / A,
		UprisingDiagonalOffset / A,
		FallingDiagonalOffset / A
	};
}

const FHhGridCellOffset& FHhGridCellOffset::operator*=(int A)
{
	HorizontalOffset *= A;
	UprisingDiagonalOffset *= A;
	FallingDiagonalOffset *= A;

	return *this;
}

const FHhGridCellOffset& FHhGridCellOffset::operator/=(int A)
{
	HorizontalOffset /= A;
	UprisingDiagonalOffset /= A;
	FallingDiagonalOffset /= A;

	return *this;
}

int FHhGridCellOffset::GetLength() const
{
	return FMath::Abs(HorizontalOffset) + FMath::Abs(UprisingDiagonalOffset) + FMath::Abs(FallingDiagonalOffset);
}

void FHhGridCellOffset::OptimizeToShortestWay()
{
	int Trinity[3] = {-HorizontalOffset, UprisingDiagonalOffset, FallingDiagonalOffset};
	Algo::Sort(Trinity);
	const int Correction = Trinity[1];

	HorizontalOffset = HorizontalOffset + Correction;
	UprisingDiagonalOffset = UprisingDiagonalOffset - Correction;
	FallingDiagonalOffset = FallingDiagonalOffset - Correction;
}
