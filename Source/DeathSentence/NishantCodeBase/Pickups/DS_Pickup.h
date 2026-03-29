// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DS_Pickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ADS_Car;

UCLASS()
class DEATHSENTENCE_API ADS_Pickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ADS_Pickup();
	virtual void Tick(float DeltaTime) override;
	virtual void ApplyTo(ADS_Car* Vehicle) PURE_VIRTUAL(ADS_Pickup::ApplyTo, );
	bool IsPickupActive() const { return bIsActive; };
    
    UPROPERTY(EditAnywhere)
    bool bIsActive = true;

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    TObjectPtr<USphereComponent> Trigger;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditAnywhere, Category = "Pickup")
    float RespawnDelay = 8.f;

    UFUNCTION()
    void HandleOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

private:	
	void ReActivate();

};
