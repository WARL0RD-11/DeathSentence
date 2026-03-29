#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class ADS_ProjectileBase;
class UArrowComponent;
class USoundBase;
class USoundAttenuation;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DEATHSENTENCE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool TryFireCannon(AActor* OptionalTarget = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool TryFireMinigun(AActor* OptionalTarget = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddCannonAmmo(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ActivateMinigun(int32 BulletCount);

	UFUNCTION(BlueprintPure, Category = "Weapon")
	float GetCannonAmmoNormalized() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	float GetMinigunAmmoNormalized() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetCannonAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetMinigunAmmo() const { return MinigunAmmo; }


	UPROPERTY(EditAnywhere, Category = "Weapon|Sounds")
	TObjectPtr<USoundBase> CannonFireSound;

	UPROPERTY(EditAnywhere, Category = "Weapon|Sounds")
	TObjectPtr<USoundAttenuation> CannonFireAttenuation;

	UPROPERTY(EditAnywhere, Category = "Weapon|Sounds")
	TObjectPtr<USoundBase> MinigunFireSound;

	UPROPERTY(EditAnywhere, Category = "Weapon|Sounds")
	TObjectPtr<USoundAttenuation> MinigunFireAttenuation;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Projectiles")
	TSubclassOf<ADS_ProjectileBase> CannonProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Projectiles")
	TSubclassOf<ADS_ProjectileBase> MinigunProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Cannon", meta = (ClampMin = "0"))
	int32 MaxAmmo = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Cannon")
	int32 CurrentAmmo = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Minigun", meta = (ClampMin = "0"))
	int32 MaxMinigunAmmo = 200;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Minigun")
	int32 MinigunAmmo = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Cannon", meta = (ClampMin = "0.0"))
	float CannonFireCooldown = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Minigun", meta = (ClampMin = "0.0"))
	float MinigunFireCooldown = 0.08f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Timing")
	float LastCannonFireTime = -1000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Timing")
	float LastMinigunFireTime = -1000.f;

	// Fallback offsets if arrow components are not found
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Fallback Spawn")
	float ForwardMuzzleOffset = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Fallback Spawn")
	float UpwardMuzzleOffset = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Minigun")
	bool bMinigunUsesTargeting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Debug")
	bool bDebugWeaponFire = true;

	// Names of Arrow Components placed on the owner actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Muzzles")
	FName CannonMuzzleComponentName = TEXT("CannonMuzzle");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Muzzles")
	FName MinigunMuzzleComponentName = TEXT("MinigunMuzzle");

	UPROPERTY()
	TObjectPtr<UArrowComponent> CannonMuzzle = nullptr;

	UPROPERTY()
	TObjectPtr<UArrowComponent> MinigunMuzzle = nullptr;

protected:
	bool FireProjectile(
		TSubclassOf<ADS_ProjectileBase> ProjectileClass,
		float FireCooldown,
		float& LastFireTimeRef,
		AActor* OptionalTarget,
		bool bUseTargeting,
		UArrowComponent* MuzzleComponent,
		USoundBase* FireSound,
		USoundAttenuation* SoundAttenuation
	);

	bool CanFire(float FireCooldown, float LastFireTime) const;

	FVector GetProjectileSpawnLocation(AActor* OwnerActor, const UArrowComponent* MuzzleComponent) const;
	FRotator GetProjectileSpawnRotation(
		AActor* OwnerActor,
		const UArrowComponent* MuzzleComponent,
		const FVector& SpawnLoc,
		AActor* OptionalTarget,
		bool bUseTargeting
	) const;

	void CacheMuzzleComponents();
	UArrowComponent* FindArrowComponentByName(AActor* OwnerActor, const FName& ComponentName) const;

	void DebugSpawn(const FVector& SpawnLoc, const FRotator& SpawnRot, const FString& Label) const;
};