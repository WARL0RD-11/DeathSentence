// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Strategy/DS_CloseThreatStrat.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/Player/DS_Car.h"

float UDS_CloseThreatStrat::ScoreTarget(AActor* Target, const UDS_EnemyBlackBoard* BlackBoard) const
{
    if (!Target || !BlackBoard || !BlackBoard->SelfCar) return -1.f;

    const float Distance = FVector::Dist(BlackBoard->SelfCar->GetActorLocation(), Target->GetActorLocation());

    return 120000.f - Distance;
}
