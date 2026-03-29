// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_DriveToTarget.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_RuntimeContext.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/EnemyAI/Controller/DS_EnemyAIController.h"
#include "NishantCodeBase/Player/DS_Car.h"
#include "DeathSentence/DeathSentencePawn.h"

void UDSBTTask_DriveToTarget::Initialize(UDSBT_RuntimeContext* InContext)
{
	Context = InContext;
}

EDSBT_Status UDSBTTask_DriveToTarget::TickNode(float DeltaTime)
{
	if (!Context || !Context->mController || !Context->Blackboard || !Context->SelfVehicle || !Context->Blackboard->CurrentTarget)
	{
		return EDSBT_Status::Failure;
	}

	ADS_Car* Car = Context->SelfVehicle;
	if (!Car)
	{
		return EDSBT_Status::Failure;
	}

	const FVector SelfLocation = Car->GetActorLocation();
	const FVector Forward = Car->GetActorForwardVector();
	const FVector TargetLocation = Context->Blackboard->CurrentTarget->GetActorLocation();
	const FVector ToTarget = (TargetLocation - SelfLocation).GetSafeNormal();

	const float Distance = FVector::Dist(SelfLocation, TargetLocation);
	const float SteeringDot = FVector::CrossProduct(Forward, ToTarget).Z;
	const float ForwardDot = FVector::DotProduct(Forward, ToTarget);
	const float SteeringInput = FMath::Clamp(SteeringDot, -1.f, 1.f);
	const float TurnAmount = FMath::Abs(SteeringInput);

	Context->Blackboard->CurrentBranchDebug = TEXT("Driving to Target");

	if (Distance <= DesiredRange)
	{
		Car->SetRecoveryAwareThrottle(0.f);
		Car->SetRecoveryAwareSteering(0.f);
		Car->SetRecoveryAwareBrake(1.f);
		return EDSBT_Status::Success;
	}

	Car->SetRecoveryAwareSteering(SteeringInput);

	const float SlowdownRadius = DesiredRange * 3.0f;

	float Throttle = 1.0f;
	float Brake = 0.0f;

	if (ForwardDot < -0.3f)
	{
		Throttle = 0.5f;
		Brake = 0.0f;
	}
	else if (ForwardDot < 0.35f)
	{
		Throttle = 0.8f;
		Brake = 0.0f;
	}
	else
	{
		Throttle = 1.0f;
		Brake = 0.0f;
	}

	Throttle *= FMath::GetMappedRangeValueClamped(
		FVector2D(0.f, 1.f),
		FVector2D(1.0f, 0.55f),
		TurnAmount
	);

	if (Distance <= SlowdownRadius)
	{
		const float ApproachScale = FMath::GetMappedRangeValueClamped(
			FVector2D(DesiredRange, SlowdownRadius),
			FVector2D(0.5f, 1.0f),
			Distance
		);

		Throttle = FMath::Min(Throttle, ApproachScale);
	}

	Car->SetRecoveryAwareBrake(Brake);
	Car->SetRecoveryAwareThrottle(Throttle);

	return EDSBT_Status::Running;
}
