// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPlane.h"

FGridCell UGridPlane::GetCellByPoint(FVector Point) const
{
	const FVector& LocalPoint = WorldToLocalPoint(Point);
	return GetCellByLocalPoint(LocalPoint);
}

FVector UGridPlane::GetCellCenterLocation(const FGridCell& Cell) const
{
	const FVector& LocalCenter = GetCellCenterLocalLocation(Cell);
	return LocalToWorldPoint(LocalCenter);
}

bool UGridPlane::IsValidCell(const FGridCell& Cell) const
{
	return
		Cell.CellColumn >= 0 &&
		Cell.CellColumn < GridSize.X &&
		Cell.CellLine >= 0 &&
		Cell.CellLine < GridSize.Y;
}

FVector UGridPlane::LocalToWorldPoint(const FVector& Point) const
{
	return GetComponentTransform().TransformPositionNoScale((Point - GetLocalCenterOffset()) * CellSize);
}

FVector UGridPlane::WorldToLocalPoint(const FVector& Point) const
{
	return GetComponentTransform().InverseTransformPositionNoScale(Point) / CellSize + GetLocalCenterOffset();
}

FGridCell UGridPlane::GetCellByLocalPoint(const FVector& Point)
{
	const float X = Point.X / (GetCellsLocalHorizontalHalfOffset() * 2);
	const float Y = Point.Y / GetCellsLocalVerticalOffset();

	const int LowestPossibleColumn = FMath::FloorToInt(X);
	const int HighestPossibleColumn = FMath::CeilToInt(X);

	const int LowestPossibleLine = FMath::FloorToInt(Y);
	const int HighestPossibleLine = FMath::CeilToInt(Y);

	FGridCell NearestCell = {};
	float NearestCellDistance = TNumericLimits<float>::Max();

	for (FGridCell Cell = FGridCell(LowestPossibleColumn, 0); Cell.GetColumn() <= HighestPossibleColumn; Cell.SetColumn(
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

FVector UGridPlane::GetCellCenterLocalLocation(const FGridCell& Cell)
{
	const int bEvenLine = Cell.GetLine() % 2;
	const float HorizontalHalfOffset = GetCellsLocalHorizontalHalfOffset();
	const float HorizontalOffset = HorizontalHalfOffset * 2.f;
	const float HorizontalLocation = HorizontalOffset * Cell.GetColumn() + HorizontalHalfOffset * bEvenLine;

	const float VerticalLocation = GetCellsLocalVerticalOffset() * Cell.GetLine();

	return FVector(HorizontalLocation, VerticalLocation, 0.f);
}

float UGridPlane::GetCellsLocalHorizontalHalfOffset()
{
	return LocalCellLongestRadius * FMath::Cos(AngleBetweenLongestAndShortestRadius);
}

float UGridPlane::GetCellsLocalVerticalOffset()
{
	return LocalCellLongestRadius * (1 + FMath::Sin(AngleBetweenLongestAndShortestRadius));
}

FVector UGridPlane::GetLocalCenterOffset() const
{
	if (!bAutoCenter)
	{
		return FVector::ZeroVector;
	}

	const FGridCell& LastCell = FGridCell(GridSize.X - 1, GridSize.Y - 1);
	return GetCellCenterLocalLocation(LastCell) / 2;
}
