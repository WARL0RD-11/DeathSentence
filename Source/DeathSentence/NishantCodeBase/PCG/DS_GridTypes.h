// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_GridTypes.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDS_CellType : uint8
{
	Empty,
	Boundary,
	Lane,
	Cover,
	Centerpiece,
	PlayerSpawn,
	EnemySpawn,
	PickupLowRisk,
	PickupHighRisk,
	PickupAmmo
};
UENUM(BlueprintType)
enum class EDS_ArenaArchetype : uint8
{
	DuelRing,
	Crossfire,
	MazeRun,
	Fortress
};

USTRUCT(BlueprintType)
struct FDS_GridCell
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Coord = FIntPoint::ZeroValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDS_CellType Type = EDS_CellType::Empty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBlocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReserved = false;
};