// Fill out your copyright notice in the Description page of Project Settings.

#include "NishantCodeBase/PCG/DS_ArenaGeneratorBase.h"
#include "NishantCodeBase/PCG/DS_ArenaArchetypeStrategy.h"
#include "NishantCodeBase/Player/DS_Car.h"

static int32 DS_CellIndex(int32 X, int32 Y, int32 Width)
{
	return Y * Width + X;
}

ADS_ArenaGeneratorBase::ADS_ArenaGeneratorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADS_ArenaGeneratorBase::BeginPlay()
{
	Super::BeginPlay();
	GenerateArena();
}

void ADS_ArenaGeneratorBase::SetLevelConfig(const FDS_LevelConfig& NewConfig)
{
	LevelConfig = NewConfig;
}

void ADS_ArenaGeneratorBase::GenerateArena()
{
	RNG.Initialize(LevelConfig.Seed);

	ResetGenerationState();
	BuildGrid();

	ApplyArchetypeMainStructure();
	ApplyArchetypeLanePass();
	ApplyArchetypeCoverPass();

	ReservePlayerSpawnZones();
	ReserveEnemySpawnZones();
	PlaceRiskRewardPickupCells();

	MaterializeArena();
}

void ADS_ArenaGeneratorBase::ResetGenerationState()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}

	SpawnedActors.Empty();
	Grid.Empty();
}

void ADS_ArenaGeneratorBase::BuildGrid()
{
	Grid.SetNum(LevelConfig.Width * LevelConfig.Height);

	for (int32 Y = 0; Y < LevelConfig.Height; ++Y)
	{
		for (int32 X = 0; X < LevelConfig.Width; ++X)
		{
			FDS_GridCell& Cell = Grid[DS_CellIndex(X, Y, LevelConfig.Width)];
			Cell.Coord = FIntPoint(X, Y);
			Cell.Type = EDS_CellType::Empty;
			Cell.bBlocked = false;
			Cell.bReserved = false;

			const bool bBoundary =
				(X == 0 || Y == 0 || X == LevelConfig.Width - 1 || Y == LevelConfig.Height - 1);

			if (bBoundary)
			{
				Cell.Type = EDS_CellType::Boundary;
				Cell.bBlocked = true;
			}
		}
	}
}

void ADS_ArenaGeneratorBase::ApplyArchetypeMainStructure()
{
	if (ArchetypeStrategy)
	{
		ArchetypeStrategy->ApplyMainStructure(Grid, LevelConfig);
	}
}

void ADS_ArenaGeneratorBase::ApplyArchetypeLanePass()
{
	if (ArchetypeStrategy)
	{
		ArchetypeStrategy->ApplyLanePass(Grid, LevelConfig);
	}
}

void ADS_ArenaGeneratorBase::ApplyArchetypeCoverPass()
{
	if (ArchetypeStrategy)
	{
		ArchetypeStrategy->ApplyCoverPass(Grid, LevelConfig, RNG);
	}
}

FDS_GridCell* ADS_ArenaGeneratorBase::GetCell(int32 X, int32 Y)
{
	if (X < 0 || Y < 0 || X >= LevelConfig.Width || Y >= LevelConfig.Height)
	{
		return nullptr;
	}

	return &Grid[DS_CellIndex(X, Y, LevelConfig.Width)];
}

const FDS_GridCell* ADS_ArenaGeneratorBase::GetCellConst(int32 X, int32 Y) const
{
	if (X < 0 || Y < 0 || X >= LevelConfig.Width || Y >= LevelConfig.Height)
	{
		return nullptr;
	}

	return &Grid[DS_CellIndex(X, Y, LevelConfig.Width)];
}

FVector ADS_ArenaGeneratorBase::CellToWorld(const FIntPoint& Coord, float Z) const
{
	return ArenaOrigin + FVector(
		Coord.X * LevelConfig.CellSize,
		Coord.Y * LevelConfig.CellSize,
		Z);
}

bool ADS_ArenaGeneratorBase::IsOpenNavigableCell(const FDS_GridCell& Cell) const
{
	return
		!Cell.bBlocked &&
		!Cell.bReserved &&
		(Cell.Type == EDS_CellType::Empty || Cell.Type == EDS_CellType::Lane);
}

bool ADS_ArenaGeneratorBase::CanUseCellForSpawn(int32 X, int32 Y, int32 ClearanceRadius) const
{
	const FDS_GridCell* CenterCell = GetCellConst(X, Y);
	if (!CenterCell)
	{
		return false;
	}

	if (!IsOpenNavigableCell(*CenterCell))
	{
		return false;
	}

	for (int32 OffsetY = -ClearanceRadius; OffsetY <= ClearanceRadius; ++OffsetY)
	{
		for (int32 OffsetX = -ClearanceRadius; OffsetX <= ClearanceRadius; ++OffsetX)
		{
			const int32 CheckX = X + OffsetX;
			const int32 CheckY = Y + OffsetY;

			const FDS_GridCell* CheckCell = GetCellConst(CheckX, CheckY);
			if (!CheckCell)
			{
				return false;
			}

			// Keep a clean pocket around the spawn.
			if (CheckCell->bBlocked || CheckCell->bReserved)
			{
				return false;
			}

			if (CheckCell->Type == EDS_CellType::Boundary ||
				CheckCell->Type == EDS_CellType::Cover ||
				CheckCell->Type == EDS_CellType::Centerpiece)
			{
				return false;
			}
		}
	}

	return true;
}

bool ADS_ArenaGeneratorBase::IsFarEnoughFromChosenCells(
	const FIntPoint& Candidate,
	const TArray<FIntPoint>& ChosenCells,
	float MinDistanceInCells) const
{
	for (const FIntPoint& Chosen : ChosenCells)
	{
		const float Dist = FVector2D::Distance(
			FVector2D((float)Candidate.X, (float)Candidate.Y),
			FVector2D((float)Chosen.X, (float)Chosen.Y));

		if (Dist < MinDistanceInCells)
		{
			return false;
		}
	}

	return true;
}

void ADS_ArenaGeneratorBase::ShuffleCells(TArray<FIntPoint>& Cells)
{
	for (int32 i = Cells.Num() - 1; i > 0; --i)
	{
		const int32 SwapIndex = RNG.RandRange(0, i);
		Cells.Swap(i, SwapIndex);
	}
}

void ADS_ArenaGeneratorBase::ReservePlayerSpawnZones()
{
	const TArray<FIntPoint> PreferredPlayerCells =
	{
		FIntPoint(3, 3),
		FIntPoint(3, 5),
		FIntPoint(5, 3)
	};

	for (const FIntPoint& Coord : PreferredPlayerCells)
	{
		if (!CanUseCellForSpawn(Coord.X, Coord.Y, 1))
		{
			continue;
		}

		if (FDS_GridCell* Cell = GetCell(Coord.X, Coord.Y))
		{
			Cell->Type = EDS_CellType::PlayerSpawn;
			Cell->bReserved = true;
			Cell->bBlocked = false;
		}
	}
}

void ADS_ArenaGeneratorBase::ReserveEnemySpawnZones()
{
	TArray<FIntPoint> CandidateCells;

	const int32 W = LevelConfig.Width;
	const int32 H = LevelConfig.Height;

	for (int32 Y = 2; Y < H - 2; ++Y)
	{
		for (int32 X = 2; X < W - 2; ++X)
		{
			const bool bTopRight = (X > (W * 2) / 3 && Y < H / 3);
			const bool bBottomLeft = (X < W / 3 && Y >(H * 2) / 3);
			const bool bBottomRight = (X > (W * 2) / 3 && Y > (H * 2) / 3);
			const bool bRightMid = (X > (W * 2) / 3 && Y >= H / 3 && Y <= (H * 2) / 3);
			const bool bBottomMid = (Y > (H * 2) / 3 && X >= W / 3 && X <= (W * 2) / 3);

			const bool bAllowedRegion =
				bTopRight || bBottomLeft || bBottomRight || bRightMid || bBottomMid;

			if (!bAllowedRegion)
			{
				continue;
			}

			if (CanUseCellForSpawn(X, Y, 1))
			{
				CandidateCells.Add(FIntPoint(X, Y));
			}
		}
	}

	ShuffleCells(CandidateCells);

	TArray<FIntPoint> ChosenEnemyCells;
	const float MinSpawnDistanceInCells = 4.0f;

	for (const FIntPoint& Candidate : CandidateCells)
	{
		if (!IsFarEnoughFromChosenCells(Candidate, ChosenEnemyCells, MinSpawnDistanceInCells))
		{
			continue;
		}

		if (FDS_GridCell* Cell = GetCell(Candidate.X, Candidate.Y))
		{
			Cell->Type = EDS_CellType::EnemySpawn;
			Cell->bReserved = true;
			Cell->bBlocked = false;
			ChosenEnemyCells.Add(Candidate);
		}
	}
}

void ADS_ArenaGeneratorBase::PlaceRiskRewardPickupCells()
{
	const int32 MidX = LevelConfig.Width / 2;
	const int32 MidY = LevelConfig.Height / 2;

	if (CanUseCellForSpawn(MidX, MidY, 0))
	{
		if (FDS_GridCell* Center = GetCell(MidX, MidY))
		{
			Center->Type = EDS_CellType::PickupHighRisk;
			Center->bReserved = true;
		}
	}

	const TArray<FIntPoint> LowRiskCandidates =
	{
		FIntPoint(2, MidY),
		FIntPoint(LevelConfig.Width - 3, MidY),
		FIntPoint(MidX, 2),
		FIntPoint(MidX, LevelConfig.Height - 3)
	};

	for (const FIntPoint& Coord : LowRiskCandidates)
	{
		if (!CanUseCellForSpawn(Coord.X, Coord.Y, 1))
		{
			continue;
		}

		if (FDS_GridCell* Cell = GetCell(Coord.X, Coord.Y))
		{
			Cell->Type = EDS_CellType::PickupLowRisk;
			Cell->bReserved = true;
		}
	}

	const TArray<FIntPoint> AmmoCandidates =
	{
		FIntPoint(2, 2),
		FIntPoint(LevelConfig.Width - 3, 2),
		FIntPoint(2, LevelConfig.Height - 3),
		FIntPoint(LevelConfig.Width - 3, LevelConfig.Height - 3)
	};

	for (const FIntPoint& Coord : AmmoCandidates)
	{
		if (!CanUseCellForSpawn(Coord.X, Coord.Y, 1))
		{
			continue;
		}

		if (FDS_GridCell* Cell = GetCell(Coord.X, Coord.Y))
		{
			Cell->Type = EDS_CellType::PickupAmmo;
			Cell->bReserved = true;
		}
	}
}

void ADS_ArenaGeneratorBase::MaterializeArena()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TArray<const FDS_GridCell*> EnemySpawnCells;

	for (const FDS_GridCell& Cell : Grid)
	{
		const FVector BaseLoc = CellToWorld(Cell.Coord, 0.f);

		if (FloorTileClass)
		{
			if (AActor* Floor = World->SpawnActor<AActor>(
				FloorTileClass,
				BaseLoc,
				FRotator::ZeroRotator))
			{
				SpawnedActors.Add(Floor);
			}
		}

		switch (Cell.Type)
		{
		case EDS_CellType::Boundary:
		{
			if (WallClass)
			{
				FActorSpawnParameters WallParams;
				WallParams.SpawnCollisionHandlingOverride =
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FRotator WallRot = FRotator::ZeroRotator;
				const bool bVerticalEdge =
					(Cell.Coord.X == 0 || Cell.Coord.X == LevelConfig.Width - 1);

				if (bVerticalEdge)
				{
					WallRot = FRotator(0.f, 90.f, 0.f);
				}

				if (AActor* Wall = World->SpawnActor<AActor>(
					WallClass,
					BaseLoc + FVector(0.f, 0.f, 100.f),
					WallRot,
					WallParams))
				{
					SpawnedActors.Add(Wall);
				}
			}
			break;
		}

		case EDS_CellType::Cover:
		{
			if (CoverClass)
			{
				if (AActor* Cover = World->SpawnActor<AActor>(
					CoverClass,
					BaseLoc,
					FRotator(0.f, RNG.FRandRange(0.f, 360.f), 0.f)))
				{
					SpawnedActors.Add(Cover);
				}
			}
			break;
		}

		case EDS_CellType::Centerpiece:
		{
			if (CenterpieceClass)
			{
				if (AActor* Center = World->SpawnActor<AActor>(
					CenterpieceClass,
					BaseLoc,
					FRotator::ZeroRotator))
				{
					SpawnedActors.Add(Center);
				}
			}
			break;
		}

		case EDS_CellType::PickupLowRisk:
		{
			if (HealthPickupClass)
			{
				if (AActor* Pickup = World->SpawnActor<AActor>(
					HealthPickupClass,
					BaseLoc + FVector(0.f, 0.f, 80.f),
					FRotator::ZeroRotator))
				{
					SpawnedActors.Add(Pickup);
				}
			}
			break;
		}

		case EDS_CellType::PickupHighRisk:
		{
			if (MinigunPickupClass)
			{
				if (AActor* Pickup = World->SpawnActor<AActor>(
					MinigunPickupClass,
					BaseLoc + FVector(0.f, 0.f, 80.f),
					FRotator::ZeroRotator))
				{
					SpawnedActors.Add(Pickup);
				}
			}
			break;
		}

		case EDS_CellType::PickupAmmo:
		{
			if (AmmoPickupClass)
			{
				if (AActor* Pickup = World->SpawnActor<AActor>(
					AmmoPickupClass,
					BaseLoc + FVector(0.f, 0.f, 80.f),
					FRotator::ZeroRotator))
				{
					SpawnedActors.Add(Pickup);
				}
			}
			break;
		}

		case EDS_CellType::EnemySpawn:
		{
			EnemySpawnCells.Add(&Cell);
			break;
		}

		default:
			break;
		}
	}

	for (int32 i = EnemySpawnCells.Num() - 1; i > 0; --i)
	{
		const int32 SwapIndex = RNG.RandRange(0, i);
		EnemySpawnCells.Swap(i, SwapIndex);
	}

	int32 SpawnedEnemyCount = 0;
	TArray<FVector> UsedEnemySpawnLocations;
	const float MinEnemySpawnDistance = LevelConfig.CellSize * 4.0f;

	for (const FDS_GridCell* SpawnCell : EnemySpawnCells)
	{
		if (!SpawnCell || SpawnedEnemyCount >= LevelConfig.EnemyCount)
		{
			break;
		}

		const FVector SpawnLoc = CellToWorld(SpawnCell->Coord, 200.f);

		bool bTooClose = false;
		for (const FVector& UsedLoc : UsedEnemySpawnLocations)
		{
			if (FVector::Dist2D(UsedLoc, SpawnLoc) < MinEnemySpawnDistance)
			{
				bTooClose = true;
				break;
			}
		}

		if (bTooClose)
		{
			continue;
		}

		const bool bUseAggressor = ((SpawnCell->Coord.X + SpawnCell->Coord.Y) % 2 == 0);
		TSubclassOf<ADS_Car> EnemyClass = bUseAggressor ? EnemyAggressorClass : EnemyDefenderClass;

		if (!EnemyClass)
		{
			continue;
		}

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (ADS_Car* Enemy = World->SpawnActor<ADS_Car>(
			EnemyClass,
			SpawnLoc,
			FRotator(0.f, 180.f, 0.f),
			Params))
		{
			SpawnedActors.Add(Enemy);
			UsedEnemySpawnLocations.Add(SpawnLoc);
			SpawnedEnemyCount++;
		}
	}
}