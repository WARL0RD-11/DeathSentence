// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/Projectile/DS_ProjectileBase.h"
#include "DS_BulletProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DEATHSENTENCE_API ADS_BulletProjectile : public ADS_ProjectileBase
{
	GENERATED_BODY()
public:
	ADS_BulletProjectile();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Movement")
	bool bBulletUseHoming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Movement", meta = (ClampMin = "0.0"))
	float BulletHomingStrength = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Impact")
	bool bBulletExplodesOnHit = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Impact")
	bool bBulletApplyRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Impact")
	bool bBulletApplyRadialImpulse = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Impact")
	bool bBulletDestroyOnHit = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Damage", meta = (ClampMin = "0.0"))
	float BulletDirectHitDamage = 6.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Damage", meta = (ClampMin = "0.0"))
	float BulletSplashDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Impact", meta = (ClampMin = "0.0"))
	float BulletExplosionRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Impact", meta = (ClampMin = "0.0"))
	float BulletImpactImpulseStrength = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Impact", meta = (ClampMin = "0.0"))
	float BulletImpactImpulseTuner = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Movement", meta = (ClampMin = "0.0"))
	float BulletInitialSpeed = 25000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Movement", meta = (ClampMin = "0.0"))
	float BulletMaxSpeed = 25000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Movement")
	float BulletGravityScale = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Lifetime", meta = (ClampMin = "0.0"))
	float BulletLifeSpan = 2.f;
	
};
