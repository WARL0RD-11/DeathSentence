// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/Projectile/DS_ProjectileBase.h"
#include "DS_CannonProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DEATHSENTENCE_API ADS_CannonProjectile : public ADS_ProjectileBase
{
	GENERATED_BODY()
	
public:
	ADS_CannonProjectile();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Movement")
	bool bCannonUseHoming = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Movement", meta = (ClampMin = "0.0"))
	float CannonHomingStrength = 1.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Impact")
	bool bCannonExplodesOnHit = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Impact")
	bool bCannonApplyRadialDamage = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Impact")
	bool bCannonApplyRadialImpulse = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Impact")
	bool bCannonDestroyOnHit = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Damage", meta = (ClampMin = "0.0"))
	float CannonDirectHitDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Damage", meta = (ClampMin = "0.0"))
	float CannonSplashDamage = 35.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Impact", meta = (ClampMin = "0.0"))
	float CannonExplosionRadius = 350.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Impact", meta = (ClampMin = "0.0"))
	float CannonImpactImpulseStrength = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Impact", meta = (ClampMin = "0.0"))
	float CannonImpactImpulseTuner = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Movement", meta = (ClampMin = "0.0"))
	float CannonInitialSpeed = 2500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Movement", meta = (ClampMin = "0.0"))
	float CannonMaxSpeed = 2500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Movement")
	float CannonGravityScale = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cannon|Lifetime", meta = (ClampMin = "0.0"))
	float CannonLifeSpan = 5.f;
};
