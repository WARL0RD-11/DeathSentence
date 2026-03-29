// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_Roam.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_RuntimeContext.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/Player/DS_Car.h"
#include "NishantCodeBase/EnemyAI/Controller/DS_EnemyAIController.h"
#include "NavigationSystem.h"

void UDSBTTask_Roam::Initialize(UDSBT_RuntimeContext* InContext)
{
	Context = InContext;
}

//ToDo Vehicle Throttle

EDSBT_Status UDSBTTask_Roam::TickNode(float DeltaTime)
{
	if (!Context || !Context->mController || !Context->Blackboard || !Context->SelfVehicle)
	{
		return EDSBT_Status::Failure;
	}

	ADS_Car* Car = Cast<ADS_Car>(Context->SelfVehicle);
	if (!Car)
	{
		return EDSBT_Status::Failure;
	}

	if (Context->Blackboard->WanderLocation.IsNearlyZero())
	{
		if (UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Context->SelfVehicle->GetWorld()))
		{
			FNavLocation Result;
			if (Nav->GetRandomReachablePointInRadius(Context->SelfVehicle->GetActorLocation(), RoamRadius, Result))
			{
				Context->Blackboard->WanderLocation = Result.Location;
			}
		}
	}

	if (Context->Blackboard->WanderLocation.IsNearlyZero())
	{
		return EDSBT_Status::Failure;
	}

	const FVector SelfLocation = Car->GetActorLocation();
	const FVector Forward = Car->GetActorForwardVector();
	const FVector WanderLocation = Context->Blackboard->WanderLocation;
	const FVector ToWander = (WanderLocation - SelfLocation).GetSafeNormal();

	const float Distance = FVector::Dist(SelfLocation, WanderLocation);
	const float SteeringDot = FVector::CrossProduct(Forward, ToWander).Z;
	const float ForwardDot = FVector::DotProduct(Forward, ToWander);
	const float SteeringInput = FMath::Clamp(SteeringDot, -1.f, 1.f);
	const float TurnAmount = FMath::Abs(SteeringInput);

	Context->Blackboard->CurrentBranchDebug = TEXT("Roaming");

	const float RoamAcceptanceRadius = 200.f;
	if (Distance <= RoamAcceptanceRadius)
	{
		Car->SetRecoveryAwareThrottle(0.f);
		Car->SetRecoveryAwareSteering(0.f);
		Car->SetRecoveryAwareBrake(1.f);

		Context->Blackboard->WanderLocation = FVector::ZeroVector;
		return EDSBT_Status::Success;
	}

	Car->SetRecoveryAwareSteering(SteeringInput);

	float Throttle = 0.7f;
	float Brake = 0.0f;

	if (ForwardDot < -0.3f)
	{
		Throttle = 0.30f;
		Brake = 0.0f;
	}
	else if (ForwardDot < 0.35f)
	{
		Throttle = 0.50f;
		Brake = 0.0f;
	}
	else
	{
		Throttle = 0.70f;
		Brake = 0.0f;
	}

	Throttle *= FMath::GetMappedRangeValueClamped(
		FVector2D(0.f, 1.f),
		FVector2D(1.0f, 0.60f),
		TurnAmount
	);

	const float SlowdownRadius = RoamAcceptanceRadius * 3.0f;
	if (Distance <= SlowdownRadius)
	{
		const float ApproachScale = FMath::GetMappedRangeValueClamped(
			FVector2D(RoamAcceptanceRadius, SlowdownRadius),
			FVector2D(0.30f, 1.0f),
			Distance
		);

		Throttle = FMath::Min(Throttle, ApproachScale);
	}

	Car->SetRecoveryAwareBrake(Brake);
	Car->SetRecoveryAwareThrottle(Throttle);

	return EDSBT_Status::Running;
}
