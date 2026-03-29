#include "NishantCodeBase/Projectile/DS_CannonProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADS_CannonProjectile::ADS_CannonProjectile()
{
}

void ADS_CannonProjectile::BeginPlay()
{
	Super::BeginPlay();

	bUseHoming = bCannonUseHoming;
	HomingStrength = CannonHomingStrength;

	bExplodesOnHit = bCannonExplodesOnHit;
	bApplyRadialDamage = bCannonApplyRadialDamage;
	bApplyRadialImpulse = bCannonApplyRadialImpulse;
	bDestroyOnHit = bCannonDestroyOnHit;

	DirectHitDamage = CannonDirectHitDamage;
	SplashDamage = CannonSplashDamage;
	ExplosionRadius = CannonExplosionRadius;
	ImpactImpulseStrength = CannonImpactImpulseStrength;
	ImpactImpulseTuner = CannonImpactImpulseTuner;

	InitialLifeSpan = CannonLifeSpan;

	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = CannonInitialSpeed;
		ProjectileMovement->MaxSpeed = CannonMaxSpeed;
		ProjectileMovement->ProjectileGravityScale = CannonGravityScale;
	}
}