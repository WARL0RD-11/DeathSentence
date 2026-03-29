// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/Pickups/DS_HealthPickup.h"
#include "NishantCodeBase/Components/HealthComponent/DS_HealthComponent.h"
#include "NishantCodeBase/Player/DS_Car.h"

void ADS_HealthPickup::ApplyTo(ADS_Car* Vehicle)
{
	if (!Vehicle) return;

	if (UDS_HealthComponent* HC = Vehicle->GetHealthComponent())
	{
		HC->Heal(HealAmount);
	}
}
