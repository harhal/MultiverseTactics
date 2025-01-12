// Fill out your copyright notice in the Description page of Project Settings.


#include "HHGridPlane.h"

FHhGridCell UHhGridPlane::GetCellByPoint(FVector Point) const
{
	const FVector& LocalPoint = WorldToLocalPoint(Point);
	return GetCellByLocalPoint(LocalPoint);
}

FVector UHhGridPlane::GetCellCenterLocation(const FHhGridCell& Cell) const
{
	const FVector& LocalCenter = GetCellCenterLocalLocation(Cell);
	return LocalToWorldPoint(LocalCenter);
}

bool UHhGridPlane::IsValidCell(const FHhGridCell& Cell) const
{
	return
		Cell.CellColumn >= 0 &&
		Cell.CellColumn < GridSize.X &&
		Cell.CellLine >= 0 &&
		Cell.CellLine < GridSize.Y;
}

FVector UHhGridPlane::LocalToWorldPoint(const FVector& Point) const
{
	return GetComponentTransform().TransformPositionNoScale((Point - GetLocalCenterOffset()) * CellSize);
}

FVector UHhGridPlane::WorldToLocalPoint(const FVector& Point) const
{
	return GetComponentTransform().InverseTransformPositionNoScale(Point) / CellSize + GetLocalCenterOffset();
}

FHhGridCell UHhGridPlane::GetCellByLocalPoint(const FVector& Point)
{
	const float X = Point.X / (GetCellsLocalHorizontalHalfOffset() * 2);
	const float Y = Point.Y / GetCellsLocalVerticalOffset();

	const int LowestPossibleColumn = FMath::FloorToInt(X);
	const int HighestPossibleColumn = FMath::CeilToInt(X);

	const int LowestPossibleLine = FMath::FloorToInt(Y);
	const int HighestPossibleLine = FMath::CeilToInt(Y);

	FHhGridCell NearestCell = {};
	float NearestCellDistance = TNumericLimits<float>::Max();

	for (FHhGridCell Cell = FHhGridCell(LowestPossibleColumn, 0); Cell.GetColumn() <= HighestPossibleColumn; Cell.SetColumn(
		     Cell.GetColumn() + 1))
	{
		for (Cell.SetLine(LowestPossibleLine); Cell.GetLine() <= HighestPossibleLine; Cell.SetLine(Cell.GetLine() + 1))
		{
			const float DistanceToCell = FVector::Dist2D(GetCellCenterLocalLocation(Cell), Point);
			if (DistanceToCell < NearestCellDistance)
			{
				NearestCell = Cell;
				NearestCellDistance = DistanceToCell;
			}
		}
	}

	return NearestCell;
}

FVector UHhGridPlane::GetCellCenterLocalLocation(const FHhGridCell& Cell)
{
	const int bEvenLine = Cell.GetLine() % 2;
	const float HorizontalHalfOffset = GetCellsLocalHorizontalHalfOffset();
	const float HorizontalOffset = HorizontalHalfOffset * 2.f;
	const float HorizontalLocation = HorizontalOffset * Cell.GetColumn() + HorizontalHalfOffset * bEvenLine;

	const float VerticalLocation = GetCellsLocalVerticalOffset() * Cell.GetLine();

	return FVector(HorizontalLocation, VerticalLocation, 0.f);
}

float UHhGridPlane::GetCellsLocalHorizontalHalfOffset()
{
	return LocalCellLongestRadius * FMath::Cos(AngleBetweenLongestAndShortestRadius);
}

float UHhGridPlane::GetCellsLocalVerticalOffset()
{
	return LocalCellLongestRadius * (1 + FMath::Sin(AngleBetweenLongestAndShortestRadius));
}

FVector UHhGridPlane::GetLocalCenterOffset() const
{
	if (!bAutoCenter)
	{
		return FVector::ZeroVector;
	}

	const FHhGridCell& LastCell = FHhGridCell(GridSize.X - 1, GridSize.Y - 1);
	return GetCellCenterLocalLocation(LastCell) / 2;
}
