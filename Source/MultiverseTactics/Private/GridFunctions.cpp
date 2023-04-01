// Fill out your copyright notice in the Description page of Project Settings.


#include "GridFunctions.h"

FGridCellOffset UGridFunctions::MakeHorizontalGridCellOffset()
{
	return FGridCellOffset::OneHorizontalOffset;
}

FGridCellOffset UGridFunctions::MakeUprisingDiagonalGridCellOffset()
{
	return FGridCellOffset::OneUprisingDiagonalOffset;
}

FGridCellOffset UGridFunctions::MakeFallingDiagonalGridCellOffset()
{
	return FGridCellOffset::OneFallingDiagonalOffset;
}

FGridCellOffset UGridFunctions::MakeDefaultDownCellOffset(bool bEven, int Length)
{
	FGridCellOffset Result = FGridCellOffset{0, - Length / 2, Length / 2};
	if (Length % 2 == 1)
	{
		if (bEven)
		{
			Result.FallingDiagonalOffset += 1;
		}
		else
		{
			Result.UprisingDiagonalOffset -= 1;
		}
	}

	return Result;
}

FGridCellOffset UGridFunctions::AddGridCellOffset(const FGridCellOffset& A, const FGridCellOffset& B)
{
	return A + B;
}

FGridCellOffset UGridFunctions::MultiplyGridCellOffset(const FGridCellOffset& A, float B)
{
	return A * B;
}

FGridCellOffset UGridFunctions::OptimizeToShortestWay(const FGridCellOffset& A)
{
	FGridCellOffset Result = A;
	Result.OptimizeToShortestWay();
	return Result;
}

int UGridFunctions::GetGridCellOffsetLength(const FGridCellOffset& Offset)
{
	return Offset.GetLength();
}

FGridCell UGridFunctions::AddOffsetToCell(const FGridCell& Cell, const FGridCellOffset& Offset)
{
	FGridCell Result;
	
	Result.CellLine = Cell.CellLine + Offset.FallingDiagonalOffset - Offset.UprisingDiagonalOffset;
	
	const int RightByDiagSum = Offset.UprisingDiagonalOffset + Offset.FallingDiagonalOffset;	
	Result.CellColumn = Cell.CellColumn + Offset.HorizontalOffset + RightByDiagSum / 2;
	
	if (Cell.CellLine % 2 == 1 == RightByDiagSum > 0)
	{
		Result.CellColumn += RightByDiagSum % 2;
	}
	
	return Result;
}

FGridCellOffset UGridFunctions::GetOffsetBetweenCells(const FGridCell& A, const FGridCell& B)
{
	FGridCellOffset Result;
	Result.FallingDiagonalOffset = B.CellLine - A.CellLine;	
	Result.HorizontalOffset = B.CellColumn - (A.CellColumn + Result.FallingDiagonalOffset / 2);
		
	if (A.CellLine % 2 == 1 == Result.FallingDiagonalOffset > 0)
	{
		Result.HorizontalOffset -= Result.FallingDiagonalOffset % 2;
	}

	Result.OptimizeToShortestWay();

	return Result;
}

void UGridFunctions::BuildStraightWay(const FGridCell& Cell, const FGridCellOffset& Offset, TArray<FGridCell>& OutWay)
{
	FGridCellOffset OptimizedOffset = OptimizeToShortestWay(Offset);
	FGridCell CurrentCell = Cell;

	OutWay.Add(CurrentCell);

	while (OptimizedOffset.HorizontalOffset != 0 ||
		OptimizedOffset.FallingDiagonalOffset != 0 ||
		OptimizedOffset.UprisingDiagonalOffset != 0)
	{
		if (OptimizedOffset.HorizontalOffset != 0)
		{
			CurrentCell.CellColumn += FMath::Sign(OptimizedOffset.HorizontalOffset);
			OptimizedOffset.HorizontalOffset -= FMath::Sign(OptimizedOffset.HorizontalOffset);
			OutWay.Add(CurrentCell);
		}
		
		if ((CurrentCell.CellLine % 2 == 0 && OptimizedOffset.FallingDiagonalOffset != 0) || OptimizedOffset.UprisingDiagonalOffset == 0)
		{
			CurrentCell = AddOffsetToCell(CurrentCell, FGridCellOffset::OneFallingDiagonalOffset * FMath::Sign(OptimizedOffset.FallingDiagonalOffset));
			OptimizedOffset.FallingDiagonalOffset -= FMath::Sign(OptimizedOffset.FallingDiagonalOffset);
			OutWay.Add(CurrentCell);
		}
		else
		{
			CurrentCell = AddOffsetToCell(CurrentCell, FGridCellOffset::OneUprisingDiagonalOffset * FMath::Sign(OptimizedOffset.UprisingDiagonalOffset));
			OptimizedOffset.UprisingDiagonalOffset -= FMath::Sign(OptimizedOffset.UprisingDiagonalOffset);
			OutWay.Add(CurrentCell);
		}
	}
}