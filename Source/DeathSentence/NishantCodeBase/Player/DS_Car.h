// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SportsCar/DeathSentenceSportsCar.h"
#include "DS_Car.generated.h"

/**
 * 
 */

class UWeaponComponent;
class UDS_HealthComponent;

UCLASS()
class DEATHSENTENCE_API ADS_Car : public ADeathSentenceSportsCar
{
	GENERATED_BODY()

public:

	ADS_Car();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintCallable)
	UDS_HealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable)
	UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

	UFUNCTION(BlueprintCallable)
	void SetRecoveryAwareThrottle(float Value);

	UFUNCTION(BlueprintCallable)
	void SetRecoveryAwareBrake(float Value);

	UFUNCTION(BlueprintCallable)
	void SetRecoveryAwareSteering(float Value);

	UFUNCTION(BlueprintCallable)
	float GetLastThrottleInput() const { return LastThrottleInput; }

	UFUNCTION(BlueprintCallable)
	float GetLastBrakeInput() const { return LastBrakeInput; }

	UFUNCTION(BlueprintCallable)
	float GetLastSteeringInput() const { return LastSteeringInput; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UDS_HealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWeaponComponent> WeaponComponent;

	UFUNCTION()
	void HandleDeath(AActor* DeadActor);

	// ---------- Flip Recovery ----------
	UPROPERTY(EditAnywhere, Category = "Recovery|Flip")
	float UpsideDownResetDelay = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Recovery|Flip")
	float UpsideDownDotThreshold = -0.2f;

	// ---------- Stuck Recovery ----------
	UPROPERTY(EditAnywhere, Category = "Recovery|Stuck")
	float StuckResetDelay = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Recovery|Stuck")
	float StuckMinSpeed = 40.f;

	UPROPERTY(EditAnywhere, Category = "Recovery|Stuck")
	float StuckMovementTolerance = 25.f;

	UPROPERTY(EditAnywhere, Category = "Recovery|Stuck")
	float StuckThrottleThreshold = 0.25f;

	// ---------- Shared Reset Location Search ----------
	UPROPERTY(EditAnywhere, Category = "Recovery|Reset")
	float ResetProbeDistance = 500.f;

	UPROPERTY(EditAnywhere, Category = "Recovery|Reset")
	float GroundTraceHeight = 500.f;

	UPROPERTY(EditAnywhere, Category = "Recovery|Reset")
	float GroundTraceDepth = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Recovery|Reset")
	float ResetHeightOffset = 80.f;

private:
	// Input tracking
	float LastThrottleInput = 0.f;
	float LastBrakeInput = 0.f;
	float LastSteeringInput = 0.f;

	// Timers
	FTimerHandle UpsideDownResetTimerHandle;
	bool bFlipTimerActive = false;

	float StuckAccumulatedTime = 0.f;

	// Stuck state tracking
	FVector LastStuckCheckLocation = FVector::ZeroVector;

private:
	bool IsClearlyUpsideDown() const;
	bool IsTryingToMove() const;
	bool IsStuck() const;

	void UpdateFlipRecovery();
	void UpdateStuckRecovery(float DeltaSeconds);

	void StartFlipRecoveryTimer();
	void CancelFlipRecoveryTimer();

	void ResetFromFlip();

	bool FindNearestResetLocation(FVector& OutLocation) const;
	void PerformRecoveryReset();
};
