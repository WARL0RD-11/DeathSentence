// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/Player/DS_Car.h"
#include "NishantCodeBase/Components/HealthComponent/DS_HealthComponent.h"
#include "NishantCodeBase/Components/WeaponComponent/WeaponComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

ADS_Car::ADS_Car()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UDS_HealthComponent>(TEXT("Health"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapons"));
}

void ADS_Car::BeginPlay()
{
	Super::BeginPlay();

	LastStuckCheckLocation = GetActorLocation();

	if (HealthComponent)
	{
		HealthComponent->OnOwnerDeath.AddDynamic(this, &ThisClass::HandleDeath);
	}

}

void ADS_Car::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsDead())
	{
		CancelFlipRecoveryTimer();
		return;
	}

	UpdateFlipRecovery();
	UpdateStuckRecovery(DeltaSeconds);
}

bool ADS_Car::IsDead() const
{
	return HealthComponent ? HealthComponent->IsDead() : true;
}

void ADS_Car::HandleDeath(AActor* DeadActor)
{
	CancelFlipRecoveryTimer();
	Destroy();
}

void ADS_Car::SetRecoveryAwareThrottle(float Value)
{
	LastThrottleInput = Value;
	DoThrottle(Value);
}

void ADS_Car::SetRecoveryAwareBrake(float Value)
{
	LastBrakeInput = Value;
	DoBrake(Value);
}

void ADS_Car::SetRecoveryAwareSteering(float Value)
{
	LastSteeringInput = Value;
	DoSteering(Value);
}

bool ADS_Car::IsClearlyUpsideDown() const
{
	return FVector::DotProduct(GetActorUpVector(), FVector::UpVector) <= UpsideDownDotThreshold;
}

bool ADS_Car::IsTryingToMove() const
{
	return FMath::Abs(LastThrottleInput) >= StuckThrottleThreshold;
}

bool ADS_Car::IsStuck() const
{
	if (!IsTryingToMove())
	{
		return false;
	}

	const FVector CurrentLocation = GetActorLocation();
	const float MovedDistance2D = FVector::Dist2D(CurrentLocation, LastStuckCheckLocation);

	return MovedDistance2D <= StuckMovementTolerance;
}

void ADS_Car::UpdateFlipRecovery()
{
	if (IsClearlyUpsideDown())
	{
		if (!bFlipTimerActive)
		{
			StartFlipRecoveryTimer();
		}
	}
	else
	{
		if (bFlipTimerActive)
		{
			CancelFlipRecoveryTimer();
		}
	}
}

void ADS_Car::UpdateStuckRecovery(float DeltaSeconds)
{

	if (IsClearlyUpsideDown())
	{
		StuckAccumulatedTime = 0.f;
		LastStuckCheckLocation = GetActorLocation();
		return;
	}

	if (!IsTryingToMove())
	{
		StuckAccumulatedTime = 0.f;
		LastStuckCheckLocation = GetActorLocation();
		return;
	}

	const FVector CurrentLocation = GetActorLocation();
	const float MovedDistance2D = FVector::Dist2D(CurrentLocation, LastStuckCheckLocation);

	if (MovedDistance2D <= StuckMovementTolerance)
	{
		StuckAccumulatedTime += DeltaSeconds;

		if (StuckAccumulatedTime >= StuckResetDelay)
		{
			PerformRecoveryReset();
			StuckAccumulatedTime = 0.f;
			LastStuckCheckLocation = GetActorLocation();
		}
	}
	else
	{
		StuckAccumulatedTime = 0.f;
		LastStuckCheckLocation = CurrentLocation;
	}
}

void ADS_Car::StartFlipRecoveryTimer()
{
	if (!GetWorld())
	{
		return;
	}

	bFlipTimerActive = true;

	GetWorldTimerManager().SetTimer(
		UpsideDownResetTimerHandle,
		this,
		&ADS_Car::ResetFromFlip,
		UpsideDownResetDelay,
		false
	);
}

void ADS_Car::CancelFlipRecoveryTimer()
{
	if (!GetWorld())
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(UpsideDownResetTimerHandle);
	bFlipTimerActive = false;
}

void ADS_Car::ResetFromFlip()
{
	bFlipTimerActive = false;

	if (IsDead())
	{
		return;
	}

	// Car recovered before timer finished
	if (!IsClearlyUpsideDown())
	{
		return;
	}

	PerformRecoveryReset();
}

bool ADS_Car::FindNearestResetLocation(FVector& OutLocation) const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	const FVector CurrentLocation = GetActorLocation();
	const FVector Forward = GetActorForwardVector().GetSafeNormal2D();
	const FVector Right = GetActorRightVector().GetSafeNormal2D();

	TArray<FVector> Candidates;
	Candidates.Add(CurrentLocation);
	Candidates.Add(CurrentLocation + Forward * ResetProbeDistance);
	Candidates.Add(CurrentLocation - Forward * ResetProbeDistance);
	Candidates.Add(CurrentLocation + Right * ResetProbeDistance);
	Candidates.Add(CurrentLocation - Right * ResetProbeDistance);
	Candidates.Add(CurrentLocation + (Forward + Right).GetSafeNormal2D() * ResetProbeDistance);
	Candidates.Add(CurrentLocation + (Forward - Right).GetSafeNormal2D() * ResetProbeDistance);
	Candidates.Add(CurrentLocation + (-Forward + Right).GetSafeNormal2D() * ResetProbeDistance);
	Candidates.Add(CurrentLocation + (-Forward - Right).GetSafeNormal2D() * ResetProbeDistance);

	bool bFoundAny = false;
	float BestDistSq = TNumericLimits<float>::Max();
	FVector BestLocation = CurrentLocation;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(CarRecoveryTrace), false, this);

	for (const FVector& Candidate : Candidates)
	{
		const FVector TraceStart = Candidate + FVector(0.f, 0.f, GroundTraceHeight);
		const FVector TraceEnd = Candidate - FVector(0.f, 0.f, GroundTraceDepth);

		FHitResult Hit;
		const bool bHit = World->LineTraceSingleByChannel(
			Hit,
			TraceStart,
			TraceEnd,
			ECC_WorldStatic,
			Params
		);

		if (!bHit)
		{
			continue;
		}

		const FVector TestLocation = Hit.ImpactPoint + FVector(0.f, 0.f, ResetHeightOffset);
		const float DistSq = FVector::DistSquared(CurrentLocation, TestLocation);

		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			BestLocation = TestLocation;
			bFoundAny = true;
		}
	}

	if (bFoundAny)
	{
		OutLocation = BestLocation;
		return true;
	}

	return false;
}

void ADS_Car::PerformRecoveryReset()
{
	FVector ResetLocation = GetActorLocation() + FVector(0.f, 0.f, ResetHeightOffset);
	FindNearestResetLocation(ResetLocation);

	const FVector Forward2D = GetActorForwardVector().GetSafeNormal2D();
	ResetLocation += Forward2D * 150.f;

	FRotator ResetRotation = GetActorRotation();
	ResetRotation.Pitch = 0.f;
	ResetRotation.Roll = 0.f;

	SetActorLocationAndRotation(
		ResetLocation,
		ResetRotation,
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);

	if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		RootPrim->SetPhysicsLinearVelocity(FVector::ZeroVector);
		RootPrim->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}

	LastStuckCheckLocation = GetActorLocation();
	LastThrottleInput = 0.f;
	LastBrakeInput = 0.f;
	LastSteeringInput = 0.f;
	StuckAccumulatedTime = 0.f;

	CancelFlipRecoveryTimer();

	DoThrottle(0.f);
	DoBrake(0.f);
	DoSteering(0.f);
}