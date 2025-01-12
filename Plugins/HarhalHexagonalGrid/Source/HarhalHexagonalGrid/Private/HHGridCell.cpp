// Fill out your copyright notice in the Description page of Project Settings.


#include "HHGridCell.h"

FHhGridCell::FHhGridCell(const int Column, const int Line): CellColumn(Column), CellLine(Line)
{}

int FHhGridCell::GetLine() const
{
	return CellLine;
}

int FHhGridCell::GetColumn() const
{
	return CellColumn;
}

void FHhGridCell::SetLine(const int Line)
{
	CellLine = Line;
}

void FHhGridCell::SetColumn(const int Column)
{
	CellColumn = Column;
}

uint32 GetTypeHash(const FHhGridCell GridCell)
{
	return GridCell.CellColumn + (GridCell.CellLine >> 16);
}
