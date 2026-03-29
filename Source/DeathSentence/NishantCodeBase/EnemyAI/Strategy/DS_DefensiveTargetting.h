// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/EnemyAI/Strategy/DS_TargettingStrategy.h"
#include "DS_DefensiveTargetting.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced)
class DEATHSENTENCE_API UDS_DefensiveTargetting : public UDS_TargettingStrategy
{
	GENERATED_BODY()

public: 
	virtual float ScoreTarget(AActor* Target, const UDS_EnemyBlackBoard* BlackBoard) const override;
	
};
