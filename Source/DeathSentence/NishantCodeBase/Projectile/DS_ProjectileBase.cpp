// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/Projectile/DS_ProjectileBase.h"
#include "NishantCodeBase/Components/HealthComponent/DS_HealthComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"

ADS_ProjectileBase::ADS_ProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(Collision);

	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);

	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	Collision->SetNotifyRigidBodyCollision(false);
	Collision->SetGenerateOverlapEvents(false);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3200.f;
	ProjectileMovement->MaxSpeed = 3200.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	Collision->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);

	InitialLifeSpan = 6.f;

	bUseHoming = false;
	HomingStrength = 1.8f;

	bExplodesOnHit = false;
	bApplyRadialDamage = false;
	bApplyRadialImpulse = false;
	bDestroyOnHit = true;
	bDebugHit = true;

	DirectHitDamage = 10.f;
	SplashDamage = 25.f;
	ExplosionRadius = 250.f;
	ImpactImpulseStrength = 1200.f;
	ImpactImpulseTuner = 1.f;
}

void ADS_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (Collision)
	{
		// Ignore the actor that fired this projectile
		if (AActor* OwnerActor = GetOwner())
		{
			Collision->IgnoreActorWhenMoving(OwnerActor, true);
		}

		// Ignore instigator too, just in case it differs from owner
		if (AActor* InstigatorActor = GetInstigator())
		{
			Collision->IgnoreActorWhenMoving(InstigatorActor, true);
		}
	}

}

void ADS_ProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bUseHoming || !TargetActor || !ProjectileMovement)
	{
		return;
	}

	const FVector ToTarget = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const float Speed = ProjectileMovement->Velocity.Size();

	ProjectileMovement->Velocity = ToTarget * Speed;
}

void ADS_ProjectileBase::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!GetWorld())
	{
		return;
	}

	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator())
	{
		return;
	}

	DebugHitInfo(OtherActor, OtherComp, Hit);

	ApplyDirectHitDamage(OtherActor);

	if (bExplodesOnHit)
	{
		ApplyExplosionEffects(Hit.ImpactPoint, OtherActor);
	}

	if (bDestroyOnHit)
	{
		Destroy();
	}
}

void ADS_ProjectileBase::ApplyDirectHitDamage(AActor* DamagedActor)
{
	if (!DamagedActor || DirectHitDamage <= 0.f)
	{
		return;
	}

	if (UDS_HealthComponent* Health = DamagedActor->FindComponentByClass<UDS_HealthComponent>())
	{
		Health->ApplyDamage(DirectHitDamage);
	}
}

void ADS_ProjectileBase::ApplyExplosionEffects(const FVector& ImpactPoint, AActor* DirectHitActor)
{
	if (!GetWorld() || ExplosionRadius <= 0.f)
	{
		return;
	}

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ProjectileExplosion), false);
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	const bool bHasOverlaps = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		ImpactPoint,
		FQuat::Identity,
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllObjects),
		Sphere,
		QueryParams
	);

	if (bHasOverlaps)
	{
		if (bApplyRadialDamage)
		{
			ApplyRadialDamageFromImpact(Overlaps, DirectHitActor);
		}

		if (bApplyRadialImpulse)
		{
			ApplyRadialImpulseFromImpact(Overlaps, ImpactPoint);
		}
	}

	if (bDebugHit)
	{
		DrawDebugSphere(GetWorld(), ImpactPoint, ExplosionRadius, 24, FColor::Orange, false, 2.0f);
	}
}

void ADS_ProjectileBase::ApplyRadialDamageFromImpact(const TArray<FOverlapResult>& Overlaps, AActor* DirectHitActor)
{
	if (SplashDamage <= 0.f)
	{
		return;
	}

	TSet<AActor*> DamagedActors;

	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* OverlapActor = Result.GetActor();
		if (!OverlapActor || OverlapActor == DirectHitActor || DamagedActors.Contains(OverlapActor))
		{
			continue;
		}

		if (UDS_HealthComponent* Health = OverlapActor->FindComponentByClass<UDS_HealthComponent>())
		{
			Health->ApplyDamage(SplashDamage);
			DamagedActors.Add(OverlapActor);
		}
	}
}

void ADS_ProjectileBase::ApplyRadialImpulseFromImpact(const TArray<FOverlapResult>& Overlaps, const FVector& ImpactPoint)
{
	const float FinalImpulseStrength = ImpactImpulseStrength * ImpactImpulseTuner;
	if (FinalImpulseStrength <= 0.f)
	{
		return;
	}

	for (const FOverlapResult& Result : Overlaps)
	{
		UPrimitiveComponent* PrimComp = Result.GetComponent();
		if (!PrimComp || !PrimComp->IsSimulatingPhysics())
		{
			continue;
		}

		PrimComp->AddRadialImpulse(
			ImpactPoint,
			ExplosionRadius,
			FinalImpulseStrength,
			ERadialImpulseFalloff::RIF_Linear,
			true
		);
	}
}

void ADS_ProjectileBase::DebugHitInfo(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit) const
{
	if (!bDebugHit)
	{
		return;
	}

	const FString HitActorName = OtherActor ? OtherActor->GetName() : TEXT("None");
	const FString HitCompName = OtherComp ? OtherComp->GetName() : TEXT("None");
	const bool bOtherCompSimulating = OtherComp ? OtherComp->IsSimulatingPhysics() : false;

	if (GetWorld())
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 18.f, 12, FColor::Red, false, 1.5f);
		DrawDebugLine(GetWorld(), GetActorLocation(), Hit.ImpactPoint, FColor::Green, false, 1.5f, 0, 1.5f);
	}
}