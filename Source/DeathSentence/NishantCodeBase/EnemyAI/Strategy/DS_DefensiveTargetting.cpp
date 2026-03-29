// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Strategy/DS_DefensiveTargetting.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/Player/DS_Car.h"

float UDS_DefensiveTargetting::ScoreTarget(AActor* Target, const UDS_EnemyBlackBoard* BlackBoard) const
{
	if (!Target || !BlackBoard->SelfCar) return -1.f;

	const float Distance = FVector::Dist(BlackBoard->SelfCar->GetActorLocation(), Target->GetActorLocation());

	const float LowHealtPenalty = (1.f - BlackBoard->HealthNormalized) * 2500.f;
	return 90000.f - Distance - LowHealtPenalty;
}
