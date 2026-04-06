// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/PCG/DS_ArenaArchetypeStrategy.h"
#include "DS_ArenaCrossfire.generated.h"

/**
 * 
 */
UCLASS()
class DEATHSENTENCE_API UDS_ArenaCrossfire : public UDS_ArenaArchetypeStrategy
{
	GENERATED_BODY()

public: 
	virtual void ApplyMainStructure(TArray<FDS_GridCell>& Grid, const
		FDS_LevelConfig& Config) override;
	virtual void ApplyLanePass(TArray<FDS_GridCell>& Grid, const
		FDS_LevelConfig& Config) override;
	virtual void ApplyCoverPass(TArray<FDS_GridCell>& Grid, const
		FDS_LevelConfig& Config, FRandomStream& RNG) override;
	
};
