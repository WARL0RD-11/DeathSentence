// Fill out your copyright notice in the Description page of Project Settings.


#include "../NishantCodeBase/Components/HealthComponent/DS_HealthComponent.h"

// Sets default values for this component's properties
UDS_HealthComponent::UDS_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDS_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UDS_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDS_HealthComponent::ApplyDamage(float InDamage)
{
	if (InDamage <= 0.f || IsDead()) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - InDamage, 0, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0)
	{
		OnOwnerDeath.Broadcast(GetOwner());
	}
}

void UDS_HealthComponent::Heal(float InHeal)
{
	if (InHeal <= 0.f || IsDead()) return;

	CurrentHealth = FMath::Clamp(CurrentHealth + InHeal, 0, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

float UDS_HealthComponent::GetNormalizedHealth() const
{
	return MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f;
}

