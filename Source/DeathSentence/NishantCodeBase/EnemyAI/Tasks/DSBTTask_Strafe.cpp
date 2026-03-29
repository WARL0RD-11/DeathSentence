// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_Strafe.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_RuntimeContext.h"
#include "NishantCodeBase/EnemyAI/Controller/DS_EnemyAIController.h"
#include "NishantCodeBase/Player/DS_Car.h"

void UDSBTTask_Strafe::Initialize(UDSBT_RuntimeContext* InContext)
{
	Context = InContext;
}

EDSBT_Status UDSBTTask_Strafe::TickNode(float DeltaTime)
{
	if (!Context || !Context->Blackboard || !Context->Blackboard->CurrentTarget || !Context->SelfVehicle)
	{
		return EDSBT_Status::Failure;
	}

	ADS_Car* Car = Cast<ADS_Car>(Context->SelfVehicle);
	if (!Car)
	{
		return EDSBT_Status::Failure;
	}

	if (!bStrafeStarted)
	{
		bStrafeStarted = true;
		StrafeTimeRemaining = StrafeDuration;
	}

	const FVector SelfLoc = Car->GetActorLocation();
	const FVector Forward = Car->GetActorForwardVector();
	const FVector TargetLoc = Context->Blackboard->CurrentTarget->GetActorLocation();

	const FVector FromTargetToSelf = SelfLoc - TargetLoc;
	const float CurrentRadius = FromTargetToSelf.Size();

	if (CurrentRadius <= KINDA_SMALL_NUMBER)
	{
		Car->SetRecoveryAwareThrottle(0.f);
		Car->SetRecoveryAwareSteering(0.f);
		Car->SetRecoveryAwareBrake(0.5f);

		bStrafeStarted = false;
		StrafeTimeRemaining = 0.f;
		return EDSBT_Status::Failure;
	}

	const FVector RadialDir = FromTargetToSelf / CurrentRadius;

	const float OrbitDirectionSign = (SideOffset >= 0.f) ? 1.f : -1.f;
	const FVector TangentDir = FVector::CrossProduct(FVector::UpVector, RadialDir).GetSafeNormal() * OrbitDirectionSign;

	const FVector DesiredOrbitCenter = TargetLoc + RadialDir * OrbitRadius;
	const FVector OrbitPoint = DesiredOrbitCenter + TangentDir * FMath::Abs(SideOffset);

	const FVector ToOrbitPoint = (OrbitPoint - SelfLoc).GetSafeNormal();
	const float DistanceToOrbitPoint = FVector::Dist(SelfLoc, OrbitPoint);

	const float SteeringDot = FVector::CrossProduct(Forward, ToOrbitPoint).Z;
	const float ForwardDot = FVector::DotProduct(Forward, ToOrbitPoint);

	Context->Blackboard->CurrentBranchDebug = TEXT("StrafeTarget");

	const float SteeringInput = FMath::Clamp(SteeringDot, -1.f, 1.f);
	const float TurnAmount = FMath::Abs(SteeringInput);

	float Throttle = 0.75f;
	float Brake = 0.f;

	if (ForwardDot < -0.3f)
	{
		Throttle = 0.35f;
		Brake = 0.0f;
	}
	else if (ForwardDot < 0.35f)
	{
		Throttle = 0.55f;
		Brake = 0.0f;
	}
	else
	{
		Throttle = 0.75f;
		Brake = 0.0f;
	}

	Throttle *= FMath::GetMappedRangeValueClamped(
		FVector2D(0.f, 1.f),
		FVector2D(1.0f, 0.55f),
		TurnAmount
	);

	if (DistanceToOrbitPoint < 300.f)
	{
		Throttle = FMath::Min(Throttle, 0.4f);
	}

	const float RadiusError = CurrentRadius - OrbitRadius;
	if (RadiusError < -150.f)
	{
		Throttle = FMath::Min(Throttle, 0.45f);
	}

	Car->SetRecoveryAwareSteering(SteeringInput);
	Car->SetRecoveryAwareBrake(Brake);
	Car->SetRecoveryAwareThrottle(Throttle);

	StrafeTimeRemaining -= DeltaTime;
	if (StrafeTimeRemaining <= 0.f)
	{
		Car->SetRecoveryAwareThrottle(0.f);
		Car->SetRecoveryAwareSteering(0.f);
		Car->SetRecoveryAwareBrake(0.f);

		bStrafeStarted = false;
		StrafeTimeRemaining = 0.f;
		return EDSBT_Status::Success;
	}

	return EDSBT_Status::Running;
}

void UDSBTTask_Strafe::ResetNode()
{
    Super::ResetNode();
    bStrafeStarted = false;
    StrafeTimeRemaining = 0.f;
}
