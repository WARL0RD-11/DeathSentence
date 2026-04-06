// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NishantCodeBase/PCG/DS_GridTypes.h"
#include "CoreMinimal.h"
#include "DS_LevelConfig.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDS_LevelConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Seed = 12345;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width = 11;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height = 11;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CellSize = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealthPickupCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoPickupCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BoostPickupCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinigunPickupCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDS_ArenaArchetype Archetype = EDS_ArenaArchetype::DuelRing;
};