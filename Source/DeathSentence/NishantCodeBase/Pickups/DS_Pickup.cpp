// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/Pickups/DS_Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NishantCodeBase/Player/DS_Car.h"
#include "TimerManager.h"

ADS_Pickup::ADS_Pickup()
{
	PrimaryActorTick.bCanEverTick = true;
    Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
    SetRootComponent(Trigger);
    Trigger->SetSphereRadius(130.f);
    Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Trigger);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADS_Pickup::HandleOverlap);
}

void ADS_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADS_Pickup::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bIsActive) return;

    if (ADS_Car* Car = Cast<ADS_Car>(OtherActor))
    {
        ApplyTo(Car);

        bIsActive = false;
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);

        FTimerHandle ReactivationTimer;
        GetWorldTimerManager().SetTimer(ReactivationTimer, this, &ThisClass::ReActivate, RespawnDelay, false);
    }
}

void ADS_Pickup::ReActivate()
{
    bIsActive = true;
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
}

void ADS_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.f, 70.f * DeltaTime, 0.f));
}

