// Fill out your copyright notice in the Description page of Project Settings.

#include "NishantCodeBase/PCG/DS_ArenaMazeRun.h"

static int32 DS_MazeIndex(int32 X, int32 Y, int32 Width)
{
	return Y * Width + X;
}

void UDS_ArenaMazeRun::ApplyMainStructure(TArray<FDS_GridCell>& Grid, const FDS_LevelConfig& Config)
{
	for (int32 Y = 1; Y < Config.Height - 1; ++Y)
	{
		for (int32 X = 1; X < Config.Width - 1; ++X)
		{
			FDS_GridCell& Cell = Grid[DS_MazeIndex(X, Y, Config.Width)];

			if (Cell.bReserved)
			{
				continue;
			}

			Cell.Type = EDS_CellType::Lane;
			Cell.bBlocked = false;
		}
	}

	const int32 CenterX = Config.Width / 2 + 1;
	const int32 CenterY = Config.Height / 2;

	FDS_GridCell& CenterCell = Grid[DS_MazeIndex(CenterX, CenterY, Config.Width)];
	if (!CenterCell.bReserved)
	{
		CenterCell.Type = EDS_CellType::Centerpiece;
		CenterCell.bBlocked = true;
	}
}

void UDS_ArenaMazeRun::ApplyLanePass(TArray<FDS_GridCell>& Grid, const FDS_LevelConfig& Config)
{
	const int32 StartX = 4;
	const int32 EndX = Config.Width - 5;

	int32 BandIndex = 0;

	for (int32 X = StartX; X <= EndX; X += 5)
	{
		const bool bBlockUpperHalf = (BandIndex % 2 == 0);

		int32 BlockStartY = bBlockUpperHalf ? 2 : (Config.Height / 2 + 2);
		int32 BlockEndY = bBlockUpperHalf ? (Config.Height / 2 - 3) : (Config.Height - 3);

		BlockStartY = FMath::Clamp(BlockStartY, 2, Config.Height - 3);
		BlockEndY = FMath::Clamp(BlockEndY, 2, Config.Height - 3);

		for (int32 Y = BlockStartY; Y <= BlockEndY; ++Y)
		{
			FDS_GridCell& Cell = Grid[DS_MazeIndex(X, Y, Config.Width)];

			if (Cell.bReserved)
			{
				continue;
			}

			if (Cell.Type == EDS_CellType::Centerpiece)
			{
				continue;
			}

			Cell.Type = EDS_CellType::Cover;
			Cell.bBlocked = true;
		}

		++BandIndex;
	}

	const TArray<int32> HorizontalRows =
	{
		Config.Height / 3,
		(Config.Height * 2) / 3
	};

	for (int32 Row : HorizontalRows)
	{
		for (int32 X = 3; X < Config.Width - 3; ++X)
		{
			const bool bLeftBlock =
				(X >= 4 && X <= 6);

			const bool bRightBlock =
				(X >= Config.Width - 7 && X <= Config.Width - 5);

			const bool bShouldBlock = bLeftBlock || bRightBlock;

			if (!bShouldBlock)
			{
				continue;
			}

			FDS_GridCell& Cell = Grid[DS_MazeIndex(X, Row, Config.Width)];

			if (Cell.bReserved)
			{
				continue;
			}

			if (Cell.Type == EDS_CellType::Centerpiece)
			{
				continue;
			}

			Cell.Type = EDS_CellType::Cover;
			Cell.bBlocked = true;
		}
	}
}

void UDS_ArenaMazeRun::ApplyCoverPass(TArray<FDS_GridCell>& Grid, const FDS_LevelConfig& Config, FRandomStream& RNG)
{
	const int32 MidX = Config.Width / 2;
	const int32 MidY = Config.Height / 2;

	const int32 SafeMinX = MidX - 5;
	const int32 SafeMaxX = MidX + 5;
	const int32 SafeMinY = MidY - 3;
	const int32 SafeMaxY = MidY + 3;

	const TArray<FIntPoint> CoverCells =
	{
		FIntPoint(MidX - 6, MidY - 4),
		FIntPoint(MidX + 6, MidY + 4),
		FIntPoint(MidX - 6, MidY + 4),
		FIntPoint(MidX + 6, MidY - 4)
	};

	for (const FIntPoint& Coord : CoverCells)
	{
		if (Coord.X < 1 || Coord.Y < 1 || Coord.X >= Config.Width - 1 || Coord.Y >= Config.Height - 1)
		{
			continue;
		}

		if (Coord.X >= SafeMinX && Coord.X <= SafeMaxX &&
			Coord.Y >= SafeMinY && Coord.Y <= SafeMaxY)
		{
			continue;
		}

		FDS_GridCell& Cell = Grid[DS_MazeIndex(Coord.X, Coord.Y, Config.Width)];

		if (Cell.bReserved)
		{
			continue;
		}

		if (Cell.Type == EDS_CellType::Centerpiece)
		{
			continue;
		}

		Cell.Type = EDS_CellType::Cover;
		Cell.bBlocked = true;
	}
}