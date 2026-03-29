// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DS_HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCarDeath, AActor*, DeadActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEATHSENTENCE_API UDS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDS_HealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float InDamage);

	UFUNCTION(BlueprintCallable)
	void Heal(float InHeal);

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return CurrentHealth <= 0.f; };

	UFUNCTION(BlueprintCallable)
	float GetNormalizedHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const { return CurrentHealth; };

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnCarDeath OnOwnerDeath;
	
protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;

private:
	float CurrentHealth = 0.f;
};
