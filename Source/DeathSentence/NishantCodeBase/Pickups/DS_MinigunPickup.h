// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/Pickups/DS_Pickup.h"
#include "DS_MinigunPickup.generated.h"

/**
 * 
 */
class ADS_Car;

UCLASS()
class DEATHSENTENCE_API ADS_MinigunPickup : public ADS_Pickup
{
	GENERATED_BODY()
public:
	virtual void ApplyTo(ADS_Car* Car) override;
 	
};
