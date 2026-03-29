// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/Pickups/DS_Pickup.h"
#include "DS_HealthPickup.generated.h"

/**
 * 
 */

UCLASS()
class DEATHSENTENCE_API ADS_HealthPickup : public ADS_Pickup
{
	GENERATED_BODY()

public:
	virtual void ApplyTo(ADS_Car* Vehicle) override;

private:
	UPROPERTY(EditAnywhere)
	float HealAmount = 35.0f;
	
};
