// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DS_TargettingStrategy.generated.h"

/**
 * 
 */

class AActor;
class UDS_EnemyBlackBoard;

UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class DEATHSENTENCE_API UDS_TargettingStrategy : public UObject
{
	GENERATED_BODY()

public: 
	virtual float ScoreTarget(AActor* Target, const UDS_EnemyBlackBoard* BlackBoard) const
		PURE_VIRTUAL(UDS_TargettingStrategy::ScoreTarget, return -1.f;);
};

UCLASS()
class DEATHSENTENCE_API UDS_AggressiveTargetting : public UDS_TargettingStrategy
{
	GENERATED_BODY()

public:
	virtual float ScoreTarget(AActor* Target, const UDS_EnemyBlackBoard* BlackBoard) const override;
};
