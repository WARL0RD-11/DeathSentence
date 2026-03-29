// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Strategy/DS_PickupStrategy.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/Pickups/DS_Pickup.h"
#include "NishantCodeBase/Pickups/DS_CannonPickup.h"
#include "NishantCodeBase/Pickups/DS_HealthPickup.h"
#include "NishantCodeBase/Player/DS_Car.h"

float UDS_Pickup_RepairPriority::ScorePickup(ADS_Pickup* Pickup, const UDS_EnemyBlackBoard* BlackBoard) const
{
	if (!Pickup || !BlackBoard || !BlackBoard->SelfCar || !Pickup->IsPickupActive()) return -1.f;

	const float Distance = FVector::Dist(BlackBoard->SelfCar->GetActorLocation(), Pickup->GetActorLocation());

	float TypeBonus = 0.f;

	if (Cast<ADS_HealthPickup>(Pickup))
	{
		TypeBonus = 5000.f;
	}
	else if(Cast<ADS_CannonPickup>(Pickup))
	{
		TypeBonus = 1000.f;
	}
	return TypeBonus + (4000.f - Distance);
}

float UDS_Pickup_ReloadPriority::ScorePickup(ADS_Pickup* Pickup, const UDS_EnemyBlackBoard* BlackBoard) const
{
	if (!Pickup || !BlackBoard || !BlackBoard->SelfCar || !Pickup->IsPickupActive()) return -1.f;

	const float Distance = FVector::Dist(BlackBoard->SelfCar->GetActorLocation(), Pickup->GetActorLocation());

	float TypeBonus = 0.f;

	if (Cast<ADS_CannonPickup>(Pickup))
	{
		TypeBonus = 5000.f;
	}
	else if (Cast<ADS_HealthPickup>(Pickup))
	{
		TypeBonus = 1000.f;
	}
	return TypeBonus + (4000.f - Distance);
}
