// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DS_ProjectileBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UDS_HealthComponent;

UCLASS()
class DEATHSENTENCE_API ADS_ProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	ADS_ProjectileBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Targeting")
	AActor* TargetActor = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Movement")
	bool bUseHoming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Movement", meta = (ClampMin = "0.0"))
	float HomingStrength = 1.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Impact")
	bool bExplodesOnHit = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Impact")
	bool bApplyRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Impact")
	bool bApplyRadialImpulse = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Impact")
	bool bDestroyOnHit = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Debug")
	bool bDebugHit = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Damage", meta = (ClampMin = "0.0"))
	float DirectHitDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Damage", meta = (ClampMin = "0.0"))
	float SplashDamage = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Impact", meta = (ClampMin = "0.0"))
	float ExplosionRadius = 250.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Impact", meta = (ClampMin = "0.0"))
	float ImpactImpulseStrength = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Impact", meta = (ClampMin = "0.0"))
	float ImpactImpulseTuner = 1.f;

protected:
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	void ApplyDirectHitDamage(AActor* DamagedActor);
	void ApplyExplosionEffects(const FVector& ImpactPoint, AActor* DirectHitActor);
	void ApplyRadialDamageFromImpact(const TArray<FOverlapResult>& Overlaps, AActor* DirectHitActor);
	void ApplyRadialImpulseFromImpact(const TArray<FOverlapResult>& Overlaps, const FVector& ImpactPoint);
	void DebugHitInfo(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit) const;
};