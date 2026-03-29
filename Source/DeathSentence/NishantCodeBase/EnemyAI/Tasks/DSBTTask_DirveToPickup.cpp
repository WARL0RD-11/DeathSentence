// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_DirveToPickup.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_RuntimeContext.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/EnemyAI/Controller/DS_EnemyAIController.h"
#include "NishantCodeBase/Pickups/DS_Pickup.h"
#include "NishantCodeBase/Player/DS_Car.h"

void UDSBTTask_DirveToPickup::Initialize(UDSBT_RuntimeContext* InContext)
{
	Context = InContext;
}

EDSBT_Status UDSBTTask_DirveToPickup::TickNode(float DeltaTime)
{
	if (!Context || !Context->mController || !Context->Blackboard || !Context->Blackboard->DesiredPickup || !Context->SelfVehicle)
	{
		return EDSBT_Status::Failure;
	}

	ADS_Pickup* Pickup = Context->Blackboard->DesiredPickup;
	if (!Pickup || !Pickup->IsPickupActive())
	{
		Context->Blackboard->DesiredPickup = nullptr;
		return EDSBT_Status::Failure;
	}

	ADS_Car* Car = Cast<ADS_Car>(Context->SelfVehicle);
	if (!Car)
	{
		return EDSBT_Status::Failure;
	}

	const FVector SelfLocation = Car->GetActorLocation();
	const FVector Forward = Car->GetActorForwardVector();
	const FVector PickupLocation = Pickup->GetActorLocation();
	const FVector ToPickup = (PickupLocation - SelfLocation).GetSafeNormal();

	const float Dist = FVector::Dist(SelfLocation, PickupLocation);
	const float SteeringDot = FVector::CrossProduct(Forward, ToPickup).Z;
	const float ForwardDot = FVector::DotProduct(Forward, ToPickup);
	const float SteeringInput = FMath::Clamp(SteeringDot, -1.f, 1.f);
	const float TurnAmount = FMath::Abs(SteeringInput);

	Context->Blackboard->CurrentBranchDebug = TEXT("DriveToPickup");

	const float PickupRange = 80.f;

	if (!Pickup->IsPickupActive())
	{
		Context->Blackboard->DesiredPickup = nullptr;
		Car->SetRecoveryAwareThrottle(0.f);
		Car->SetRecoveryAwareSteering(0.f);
		Car->SetRecoveryAwareBrake(0.f);
		return EDSBT_Status::Success;
	}

	Car->SetRecoveryAwareSteering(SteeringInput);

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

	const float SlowdownRadius = PickupRange * 3.0f;
	if (Dist <= SlowdownRadius)
	{
		const float ApproachScale = FMath::GetMappedRangeValueClamped(
			FVector2D(PickupRange, SlowdownRadius),
			FVector2D(0.5f, 1.0f),
			Dist
		);

		Throttle = FMath::Min(Throttle, ApproachScale);
	}

	Car->SetRecoveryAwareBrake(Brake);
	Car->SetRecoveryAwareThrottle(Throttle);

	return EDSBT_Status::Running;
}