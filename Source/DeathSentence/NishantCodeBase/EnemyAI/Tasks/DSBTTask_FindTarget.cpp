// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_FindTarget.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_RuntimeContext.h"
#include "NishantCodeBase/EnemyAI/Controller/DS_EnemyAIController.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/EnemyAI/Strategy/DS_TargettingStrategy.h"
#include "NishantCodeBase/Player/DS_Car.h"
#include "Kismet/GameplayStatics.h"


void UDSBTTask_FindTarget::Initialize(UDSBT_RuntimeContext* InContext)
{
	Context = InContext;
}

EDSBT_Status UDSBTTask_FindTarget::TickNode(float DeltaTime)
{
	if (!Context || !Context->mController || !Context->Blackboard)
	{
		return EDSBT_Status::Failure;
	}

	TimeUntilRetarget -= DeltaTime;

	AActor* CurrentTarget = Context->Blackboard->CurrentTarget;

	// Keep current target for a short time if it is still valid.
	if (TimeUntilRetarget > 0.f)
	{
		if (CurrentTarget)
		{
			const ADS_Car* CurrentCar = Cast<ADS_Car>(CurrentTarget);
			if (CurrentCar && !CurrentCar->IsDead())
			{
				Context->Blackboard->LastKnownTargetLocation = CurrentTarget->GetActorLocation();
				Context->Blackboard->CurrentBranchDebug = TEXT("Keeping Current Target");
				return EDSBT_Status::Success;
			}
		}
	}

	TArray<AActor*> FoundTargets;
	UGameplayStatics::GetAllActorsOfClass(Context->mController, ADS_Car::StaticClass(), FoundTargets);

	float BestScore = -FLT_MAX;
	AActor* BestTarget = nullptr;

	UDS_TargettingStrategy* Strategy = Context->mController->GetThreatStrategy();
	if (!Strategy)
	{
		return EDSBT_Status::Failure;
	}

	for (AActor* Actor : FoundTargets)
	{
		if (!Actor || Actor == Context->Blackboard->SelfCar)
		{
			continue;
		}

		const ADS_Car* Car = Cast<ADS_Car>(Actor);
		if (!Car || Car->IsDead())
		{
			continue;
		}

		float Score = Strategy->ScoreTarget(Actor, Context->Blackboard);

		// Prefer the player over AI targets.
		if (Car->IsPlayerControlled())
		{
			Score += 1500.f;
		}

		// Penalize the current target a bit so the AI does not tunnel forever.
		if (Actor == CurrentTarget)
		{
			Score -= CurrentTargetScorePenalty;
		}

		if (Score > BestScore)
		{
			BestScore = Score;
			BestTarget = Actor;
		}
	}

	Context->Blackboard->CurrentTarget = BestTarget;

	if (BestTarget)
	{
		Context->Blackboard->LastKnownTargetLocation = BestTarget->GetActorLocation();
		Context->Blackboard->CurrentBranchDebug = TEXT("Switching / Finding Target");
		TimeUntilRetarget = RetargetInterval;
		return EDSBT_Status::Success;
	}

	return EDSBT_Status::Failure;
}

void UDSBTTask_FindTarget::ResetNode()
{
	TimeUntilRetarget = 0.f;
}
