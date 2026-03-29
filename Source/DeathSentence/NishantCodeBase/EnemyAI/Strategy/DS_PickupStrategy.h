// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DS_PickupStrategy.generated.h"

/**
 * 
 */
class ADS_Pickup;
class UDS_EnemyBlackBoard;

UCLASS(EditInlineNew, DefaultToInstanced)
class DEATHSENTENCE_API UDS_PickupStrategy : public UObject
{
	GENERATED_BODY()
public:
	virtual float ScorePickup(ADS_Pickup* Pickup, const UDS_EnemyBlackBoard* BlackBoard) const
		PURE_VIRTUAL(&ThisClass::ScorePickup, return -1.f;);
};

UCLASS()
class DEATHSENTENCE_API UDS_Pickup_RepairPriority : public UDS_PickupStrategy
{
	GENERATED_BODY()
public: 
	virtual float ScorePickup(ADS_Pickup* Pickup, const UDS_EnemyBlackBoard* BlackBoard) const override;
};

UCLASS()
class DEATHSENTENCE_API UDS_Pickup_ReloadPriority : public UDS_PickupStrategy
{
	GENERATED_BODY()
public:
	virtual float ScorePickup(ADS_Pickup* Pickup, const UDS_EnemyBlackBoard* BlackBoard) const override;
};