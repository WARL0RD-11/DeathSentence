// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/PCG/DS_ArenaDirector.h"
#include "NishantCodeBase/PCG/DS_ArenaGeneratorBase.h"
#include "NishantCodeBase/PCG/DS_ArenaCrossfire.h"
#include "NishantCodeBase/PCG/DS_ArenaMazeRun.h"

void ADS_ArenaDirector::BeginPlay()
{
	Super::BeginPlay();
}

void ADS_ArenaDirector::BuildLevel(int32 LevelIndex)
{
	if (!ArenaGenerator)
	{
		UE_LOG(LogTemp, Warning, TEXT("ArenaDirector: No ArenaGenerator assigned!"));
		return;
	}

	FDS_LevelConfig Config;
	Config.LevelIndex = FMath::Max(1, LevelIndex);
	Config.Seed = 1000 + Config.LevelIndex * 131; 
	Config.Width = 11 + FMath::Min(Config.LevelIndex / 2, 4);
	Config.Height = 11 + FMath::Min(Config.LevelIndex / 2, 4);
	Config.EnemyCount = FMath::Clamp(2 + (Config.LevelIndex + 3), 3, 8);
	Config.HealthPickupCount = 3;
	Config.AmmoPickupCount = 2 + (Config.LevelIndex >= 3 ? 1 : 0);
	Config.BoostPickupCount = 1 + (Config.LevelIndex >= 5 ? 1 : 0);
	Config.MinigunPickupCount = 1;

	const int32 ArchetypeChoice = (Config.LevelIndex - 1) % 2;
	
	switch (ArchetypeChoice)
	{
	case 0:
		Config.Archetype = EDS_ArenaArchetype::MazeRun;
		ArenaGenerator->ArchetypeStrategy =
			NewObject<UDS_ArenaMazeRun>(ArenaGenerator);
		break;
	case 1:
		Config.Archetype = EDS_ArenaArchetype::Crossfire;
		ArenaGenerator->ArchetypeStrategy =
			NewObject<UDS_ArenaCrossfire>(ArenaGenerator);
		break;
	default:
		Config.Archetype = EDS_ArenaArchetype::Crossfire;
		ArenaGenerator->ArchetypeStrategy =
			NewObject<UDS_ArenaCrossfire>(ArenaGenerator);
		break;
	}
	ArenaGenerator->SetLevelConfig(Config);
	ArenaGenerator->GenerateArena();
}



