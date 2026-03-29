#include "NishantCodeBase/Projectile/DS_BulletProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADS_BulletProjectile::ADS_BulletProjectile()
{
}

void ADS_BulletProjectile::BeginPlay()
{
	Super::BeginPlay();

	bUseHoming = bBulletUseHoming;
	HomingStrength = BulletHomingStrength;

	bExplodesOnHit = bBulletExplodesOnHit;
	bApplyRadialDamage = bBulletApplyRadialDamage;
	bApplyRadialImpulse = bBulletApplyRadialImpulse;
	bDestroyOnHit = bBulletDestroyOnHit;

	DirectHitDamage = BulletDirectHitDamage;
	SplashDamage = BulletSplashDamage;
	ExplosionRadius = BulletExplosionRadius;
	ImpactImpulseStrength = BulletImpactImpulseStrength;
	ImpactImpulseTuner = BulletImpactImpulseTuner;

	InitialLifeSpan = BulletLifeSpan;

	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = BulletInitialSpeed;
		ProjectileMovement->MaxSpeed = BulletMaxSpeed;
		ProjectileMovement->ProjectileGravityScale = BulletGravityScale;
	}
}