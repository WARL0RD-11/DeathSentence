// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/PCG/DS_GridTypes.h"
#include "NishantCodeBase/PCG/DS_LevelConfig.h"
#include "UObject/NoExportTypes.h"
#include "DS_ArenaArchetypeStrategy.generated.h"

/**
 * 
 */

UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class DEATHSENTENCE_API UDS_ArenaArchetypeStrategy : public UObject
{
	GENERATED_BODY()
public:
	virtual void ApplyMainStructure(TArray<FDS_GridCell>& Grid, const
		FDS_LevelConfig& Config)
		PURE_VIRTUAL(UDS_ArenaArchetypeStrategy::ApplyMainStructure, );

	virtual void ApplyLanePass(TArray<FDS_GridCell>& Grid, const
		FDS_LevelConfig& Config)
		PURE_VIRTUAL(UDS_ArenaArchetypeStrategy::ApplyLanePass, );

	virtual void ApplyCoverPass(TArray<FDS_GridCell>& Grid, const
		FDS_LevelConfig& Config, FRandomStream& RNG)
		PURE_VIRTUAL(UDS_ArenaArchetypeStrategy::ApplyCoverPass, );
};
