// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/PCG/DS_ArenaCrossfire.h"

static int32 DS_Index(int32 X, int32 Y, int32 Width)
{
	return Y * Width + X;
}

void UDS_ArenaCrossfire::ApplyMainStructure(TArray<FDS_GridCell>& Grid, const FDS_LevelConfig& Config)
{
	const int32 MidX = Config.Width / 2;
	const int32 MidY = Config.Height / 2;

	for (int32 Y = 1; Y < Config.Height - 1; Y++)
	{
		FDS_GridCell& Cell = Grid[DS_Index(MidX, Y, Config.Width)];
		if (!Cell.bReserved)
		{
			Cell.Type = EDS_CellType::Lane;
			Cell.bBlocked = false;
		}
	}

	for (int32 X = 1; X < Config.Width - 1; X++)
	{
		FDS_GridCell& Cell = Grid[DS_Index(X, MidY, Config.Width)];
		if (!Cell.bReserved)
		{
			Cell.Type = EDS_CellType::Lane;
			Cell.bBlocked = false;
		}
	}

	FDS_GridCell& CenterCell = Grid[DS_Index(MidX, MidY, Config.Width)];
	if (!CenterCell.bReserved)
	{
		CenterCell.Type = EDS_CellType::Centerpiece;
	}
}

void UDS_ArenaCrossfire::ApplyLanePass(TArray<FDS_GridCell>& Grid, const FDS_LevelConfig& Config)
{
	const int32 MidX = Config.Width / 2;
	const int32 MidY = Config.Height / 2;

	for (int32 Y = MidY - 1; Y <= MidY + 1; ++Y)
	{
		for (int32 X = 1; X < Config.Width - 1; ++X)
		{
			FDS_GridCell& Cell = Grid[DS_Index(X, Y, Config.Width)];
			if (Cell.bReserved) continue;

			Cell.Type = EDS_CellType::Lane;
			Cell.bBlocked = false;
		}
	}

	for (int32 X = MidX - 1; X <= MidX + 1; ++X)
	{
		for (int32 Y = 1; Y < Config.Height - 1; ++Y)
		{
			FDS_GridCell& Cell = Grid[DS_Index(X, Y, Config.Width)];
			if (Cell.bReserved) continue;

			Cell.Type = EDS_CellType::Lane;
			Cell.bBlocked = false;
		}
	}

}

void UDS_ArenaCrossfire::ApplyCoverPass(TArray<FDS_GridCell>& Grid, const FDS_LevelConfig& Config, FRandomStream& RNG)
{
	const TArray<FIntPoint> CoverCells =
	{
		FIntPoint(5, 5),
		FIntPoint(Config.Width - 6, 5),
		FIntPoint(5, Config.Height - 6),
		FIntPoint(Config.Width - 6, Config.Height - 6)
	};

	for (const FIntPoint& Coord : CoverCells)
	{
		FDS_GridCell& Cell = Grid[DS_Index(Coord.X, Coord.Y, Config.Width)];
		if (Cell.bReserved) continue;

		Cell.Type = EDS_CellType::Cover;
		Cell.bBlocked = true;
	}
}