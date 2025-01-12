// Fill out your copyright notice in the Description page of Project Settings.


#include "HHGridFunctions.h"

FHhGridCellOffset UHhGridFunctions::MakeHorizontalGridCellOffset()
{
	return FHhGridCellOffset::OneHorizontalOffset;
}

FHhGridCellOffset UHhGridFunctions::MakeUprisingDiagonalGridCellOffset()
{
	return FHhGridCellOffset::OneUprisingDiagonalOffset;
}

FHhGridCellOffset UHhGridFunctions::MakeFallingDiagonalGridCellOffset()
{
	return FHhGridCellOffset::OneFallingDiagonalOffset;
}

FHhGridCellOffset UHhGridFunctions::MakeDefaultDownCellOffset(bool bEven, int Length)
{
	FHhGridCellOffset Result = FHhGridCellOffset{0, - Length / 2, Length / 2};
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

FHhGridCellOffset UHhGridFunctions::AddGridCellOffset(const FHhGridCellOffset& A, const FHhGridCellOffset& B)
{
	return A + B;
}

FHhGridCellOffset UHhGridFunctions::MultiplyGridCellOffset(const FHhGridCellOffset& A, float B)
{
	return A * B;
}

FHhGridCellOffset UHhGridFunctions::OptimizeToShortestWay(const FHhGridCellOffset& A)
{
	FHhGridCellOffset Result = A;
	Result.OptimizeToShortestWay();
	return Result;
}

int UHhGridFunctions::GetGridCellOffsetLength(const FHhGridCellOffset& Offset)
{
	return Offset.GetLength();
}

FHhGridCell UHhGridFunctions::AddOffsetToCell(const FHhGridCell& Cell, const FHhGridCellOffset& Offset)
{
	FHhGridCell Result;
	
	Result.CellLine = Cell.CellLine + Offset.FallingDiagonalOffset - Offset.UprisingDiagonalOffset;
	
	const int RightByDiagSum = Offset.UprisingDiagonalOffset + Offset.FallingDiagonalOffset;	
	Result.CellColumn = Cell.CellColumn + Offset.HorizontalOffset + RightByDiagSum / 2;
	
	if (Cell.CellLine % 2 == 1 == RightByDiagSum > 0)
	{
		Result.CellColumn += RightByDiagSum % 2;
	}
	
	return Result;
}

FHhGridCellOffset UHhGridFunctions::GetOffsetBetweenCells(const FHhGridCell& A, const FHhGridCell& B)
{
	FHhGridCellOffset Result;
	Result.FallingDiagonalOffset = B.CellLine - A.CellLine;	
	Result.HorizontalOffset = B.CellColumn - (A.CellColumn + Result.FallingDiagonalOffset / 2);
		
	if (A.CellLine % 2 == 1 == Result.FallingDiagonalOffset > 0)
	{
		Result.HorizontalOffset -= Result.FallingDiagonalOffset % 2;
	}

	Result.OptimizeToShortestWay();

	return Result;
}

void UHhGridFunctions::BuildStraightWay(const FHhGridCell& Cell, const FHhGridCellOffset& Offset, TArray<FHhGridCell>& OutWay)
{
	FHhGridCellOffset OptimizedOffset = OptimizeToShortestWay(Offset);
	FHhGridCell CurrentCell = Cell;

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

		if (OptimizedOffset.FallingDiagonalOffset == 0 &&
			OptimizedOffset.UprisingDiagonalOffset == 0)
		{
			continue;
		}
		
		if ((CurrentCell.CellLine % 2 == 0 && OptimizedOffset.FallingDiagonalOffset != 0) || OptimizedOffset.UprisingDiagonalOffset == 0)
		{
			CurrentCell = AddOffsetToCell(CurrentCell, FHhGridCellOffset::OneFallingDiagonalOffset * FMath::Sign(OptimizedOffset.FallingDiagonalOffset));
			OptimizedOffset.FallingDiagonalOffset -= FMath::Sign(OptimizedOffset.FallingDiagonalOffset);
			OutWay.Add(CurrentCell);
		}
		else
		{
			CurrentCell = AddOffsetToCell(CurrentCell, FHhGridCellOffset::OneUprisingDiagonalOffset * FMath::Sign(OptimizedOffset.UprisingDiagonalOffset));
			OptimizedOffset.UprisingDiagonalOffset -= FMath::Sign(OptimizedOffset.UprisingDiagonalOffset);
			OutWay.Add(CurrentCell);
		}
	}
}