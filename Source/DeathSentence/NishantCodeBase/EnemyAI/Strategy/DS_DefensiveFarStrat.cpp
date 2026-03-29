// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Strategy/DS_DefensiveFarStrat.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/Player/DS_Car.h"

float UDS_DefensiveFarStrat::ScoreTarget(AActor* Target, const UDS_EnemyBlackBoard* BlackBoard) const
{
    if (!Target || !BlackBoard || !BlackBoard->SelfCar)
    {
        return -1.f;
    }

    const float Dist = FVector::Dist(
        BlackBoard->SelfCar->GetActorLocation(),
        Target->GetActorLocation());

    const float PreferredRange = 2200.f;
    const float RangeScore = -FMath::Abs(Dist - PreferredRange);
    const float SurvivalBias = BlackBoard->HealthNormalized * 800.f;

    return 100000.f + RangeScore + SurvivalBias;
}
