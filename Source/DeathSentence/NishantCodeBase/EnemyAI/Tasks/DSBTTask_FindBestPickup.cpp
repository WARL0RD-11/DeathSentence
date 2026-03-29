// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_FindBestPickup.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_RuntimeContext.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/EnemyAI/Strategy/DS_PickupStrategy.h"
#include "NishantCodeBase/Pickups/DS_Pickup.h"
#include "NishantCodeBase/Player/DS_Car.h"
#include "Kismet/GameplayStatics.h"


void UDSBTTask_FindBestPickup::Initialize(UDSBT_RuntimeContext* InContext)
{
	Context = InContext;
}

EDSBT_Status UDSBTTask_FindBestPickup::TickNode(float DeltaTime)
{
	if (!Context || !Context->Blackboard || !PickupStrat) return EDSBT_Status::Failure;

	TArray<AActor*> FoundPickups;
	UGameplayStatics::GetAllActorsOfClass(Context->SelfVehicle, ADS_Pickup::StaticClass(), FoundPickups);

	float BestScore = -FLT_MAX;
	ADS_Pickup* BestPickup = nullptr;

	for (AActor* Actor : FoundPickups)
	{
		ADS_Pickup* Pickup = Cast<ADS_Pickup>(Actor);
		if (!Pickup || !Pickup->bIsActive) continue;

		const float Score = PickupStrat->ScorePickup(Pickup, Context->Blackboard);
		if (Score > BestScore)
		{
			BestScore = Score;
			BestPickup = Pickup;
		}
	}

	Context->Blackboard->DesiredPickup = BestPickup;
	if (BestPickup)
	{
		Context->Blackboard->CurrentBranchDebug = TEXT("Finding Best Pickup");
		return EDSBT_Status::Success;
	}

	return EDSBT_Status::Failure;
}
