// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/Pickups/DS_CannonPickup.h"
#include "NishantCodeBase/Player/DS_Car.h"
#include "NishantCodeBase/Components/WeaponComponent/WeaponComponent.h"

void ADS_CannonPickup::ApplyTo(ADS_Car* Vehicle)
{
	if (!Vehicle) return;

	if (UWeaponComponent* WC = Vehicle->GetWeaponComponent())
	{
		WC->AddCannonAmmo(ReloadAmount);
	}
}
