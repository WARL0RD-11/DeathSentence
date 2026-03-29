#include "NishantCodeBase/Components/WeaponComponent/WeaponComponent.h"
#include "NishantCodeBase/Projectile/DS_ProjectileBase.h"

#include "Components/ArrowComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/Engine.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
	MinigunAmmo = 0;

	CacheMuzzleComponents();
}

void UWeaponComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UWeaponComponent::TryFireCannon(AActor* OptionalTarget)
{
	if (CurrentAmmo <= 0)
	{
		return false;
	}

	const bool bFired = FireProjectile(
		CannonProjectileClass,
		CannonFireCooldown,
		LastCannonFireTime,
		OptionalTarget,
		true,
		CannonMuzzle,
		CannonFireSound,
		CannonFireAttenuation
	);

	if (bFired)
	{
		--CurrentAmmo;
	}

	return bFired;
}

bool UWeaponComponent::TryFireMinigun(AActor* OptionalTarget)
{
	if (MinigunAmmo <= 0)
	{
		return false;
	}

	const bool bFired = FireProjectile(
		MinigunProjectileClass,
		MinigunFireCooldown,
		LastMinigunFireTime,
		OptionalTarget,
		bMinigunUsesTargeting,
		MinigunMuzzle,
		MinigunFireSound,
		MinigunFireAttenuation
	);

	if (bFired)
	{
		--MinigunAmmo;

		if (GEngine && MinigunAmmo % 25 == 0)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				1.0f,
				FColor::Orange,
				FString::Printf(TEXT("Minigun Ammo: %d"), MinigunAmmo)
			);
		}
	}

	return bFired;
}

bool UWeaponComponent::FireProjectile(
	TSubclassOf<ADS_ProjectileBase> ProjectileClass,
	float FireCooldown,
	float& LastFireTimeRef,
	AActor* OptionalTarget,
	bool bUseTargeting,
	UArrowComponent* MuzzleComponent,
	USoundBase* FireSound,
	USoundAttenuation* SoundAttenuation)
{
	UWorld* World = GetWorld();
	AActor* OwnerActor = GetOwner();

	if (!World || !OwnerActor)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Yellow,
				TEXT("WeaponComponent: World or Owner is null")
			);
		}
		return false;
	}

	if (!ProjectileClass)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Yellow,
				TEXT("WeaponComponent: ProjectileClass is null")
			);
		}
		return false;
	}

	if (!CanFire(FireCooldown, LastFireTimeRef))
	{
		return false;
	}

	const FVector SpawnLoc = GetProjectileSpawnLocation(OwnerActor, MuzzleComponent);
	const FRotator SpawnRot = GetProjectileSpawnRotation(OwnerActor, MuzzleComponent, SpawnLoc, OptionalTarget, bUseTargeting);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerActor->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ADS_ProjectileBase* SpawnedProjectile = World->SpawnActor<ADS_ProjectileBase>(
		ProjectileClass,
		SpawnLoc,
		SpawnRot,
		SpawnParams
	);

	if (!SpawnedProjectile)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Red,
				TEXT("WeaponComponent: Failed to spawn projectile")
			);
		}
		return false;
	}

	if (OptionalTarget && bUseTargeting)
	{
		SpawnedProjectile->TargetActor = OptionalTarget;
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			FireSound,
			SpawnLoc,
			1.0f,
			1.0f,
			0.0f,
			SoundAttenuation
		);
	}

	if (UProjectileMovementComponent* MoveComp = SpawnedProjectile->FindComponentByClass<UProjectileMovementComponent>())
	{
		const FVector Forward = SpawnRot.Vector();

		// Launch bullet immediately at full speed
		MoveComp->Velocity = Forward * MoveComp->InitialSpeed;

		// Optional: inherit a small bit of vehicle velocity
		MoveComp->Velocity += OwnerActor->GetVelocity() * 0.2f;
	}

	LastFireTimeRef = World->GetTimeSeconds();

	const FString Label = MuzzleComponent ? MuzzleComponent->GetName() : TEXT("FallbackSpawn");
	DebugSpawn(SpawnLoc, SpawnRot, Label);

	return true;
}



bool UWeaponComponent::CanFire(float FireCooldown, float LastFireTime) const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	const float CurrentTime = World->GetTimeSeconds();
	return (CurrentTime - LastFireTime) >= FireCooldown;
}

FVector UWeaponComponent::GetProjectileSpawnLocation(AActor* OwnerActor, const UArrowComponent* MuzzleComponent) const
{
	check(OwnerActor);

	if (MuzzleComponent)
	{
		return MuzzleComponent->GetComponentLocation();
	}

	return OwnerActor->GetActorLocation()
		+ OwnerActor->GetActorForwardVector() * ForwardMuzzleOffset
		+ FVector(0.f, 0.f, UpwardMuzzleOffset);
}

FRotator UWeaponComponent::GetProjectileSpawnRotation(
	AActor* OwnerActor,
	const UArrowComponent* MuzzleComponent,
	const FVector& SpawnLoc,
	AActor* OptionalTarget,
	bool bUseTargeting) const
{
	check(OwnerActor);

	if (OptionalTarget && bUseTargeting)
	{
		return (OptionalTarget->GetActorLocation() - SpawnLoc).Rotation();
	}

	if (MuzzleComponent)
	{
		return MuzzleComponent->GetComponentRotation();
	}

	return OwnerActor->GetActorRotation();
}

void UWeaponComponent::CacheMuzzleComponents()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	CannonMuzzle = FindArrowComponentByName(OwnerActor, CannonMuzzleComponentName);
	MinigunMuzzle = FindArrowComponentByName(OwnerActor, MinigunMuzzleComponentName);
}

UArrowComponent* UWeaponComponent::FindArrowComponentByName(AActor* OwnerActor, const FName& ComponentName) const
{
	if (!OwnerActor || ComponentName.IsNone())
	{
		return nullptr;
	}

	TArray<UArrowComponent*> ArrowComponents;
	OwnerActor->GetComponents<UArrowComponent>(ArrowComponents);

	for (UArrowComponent* ArrowComp : ArrowComponents)
	{
		if (ArrowComp && ArrowComp->GetFName() == ComponentName)
		{
			return ArrowComp;
		}
	}

	return nullptr;
}

void UWeaponComponent::DebugSpawn(const FVector& SpawnLoc, const FRotator& SpawnRot, const FString& Label) const
{
	if (!bDebugWeaponFire || !GetWorld())
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SpawnLoc, 16.f, 12, FColor::Red, false, 1.0f);
	DrawDebugLine(
		GetWorld(),
		SpawnLoc,
		SpawnLoc + SpawnRot.Vector() * 250.f,
		FColor::Green,
		false,
		1.0f,
		0,
		2.f
	);

}

void UWeaponComponent::AddCannonAmmo(int32 Amount)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo + Amount, 0, MaxAmmo);
}

void UWeaponComponent::ActivateMinigun(int32 BulletCount)
{
	MinigunAmmo = FMath::Clamp(MinigunAmmo + BulletCount, 0, MaxMinigunAmmo);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Orange,
			FString::Printf(TEXT("Minigun Ready! Ammo: %d"), MinigunAmmo)
		);
	}
}

float UWeaponComponent::GetCannonAmmoNormalized() const
{
	return MaxAmmo > 0 ? static_cast<float>(CurrentAmmo) / static_cast<float>(MaxAmmo) : 0.f;
}

float UWeaponComponent::GetMinigunAmmoNormalized() const
{
	return MaxMinigunAmmo > 0 ? static_cast<float>(MinigunAmmo) / static_cast<float>(MaxMinigunAmmo) : 0.f;
}