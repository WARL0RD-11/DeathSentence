// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/Pickups/DS_Pickup.h"
#include "DS_CannonPickup.generated.h"

/**
 * 
 */
UCLASS()
class DEATHSENTENCE_API ADS_CannonPickup : public ADS_Pickup
{
	GENERATED_BODY()

public:
	virtual void ApplyTo(ADS_Car* Vehicle) override;

private:
	UPROPERTY(EditAnywhere)
	int32 ReloadAmount = 10;
	
};
