// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Strategy/DS_TargettingStrategy.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/Player/DS_Car.h"
#include "GameFramework/Actor.h"

float UDS_AggressiveTargetting::ScoreTarget(AActor* Target, const UDS_EnemyBlackBoard* BlackBoard) const
{
	if (!Target || !BlackBoard || !BlackBoard->SelfCar) return -1.f;

	const float Distance = FVector::Dist(
		BlackBoard->SelfCar->GetActorLocation(), Target->GetActorLocation());

	return 100000.f - Distance;
}
