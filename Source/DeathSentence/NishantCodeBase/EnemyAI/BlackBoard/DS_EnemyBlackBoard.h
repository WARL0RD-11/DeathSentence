// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DS_EnemyBlackBoard.generated.h"

/**
 * 
 */
class ADS_Car;
class AActor;
class ADS_Pickup;

UCLASS()
class DEATHSENTENCE_API UDS_EnemyBlackBoard : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ADS_Car> SelfCar;

    UPROPERTY()
    TObjectPtr<AActor> CurrentTarget;

    UPROPERTY()
    TObjectPtr<ADS_Pickup> DesiredPickup;

    UPROPERTY()
    FVector LastKnownTargetLocation = FVector::ZeroVector;

    UPROPERTY()
    FVector WanderLocation = FVector::ZeroVector;

    UPROPERTY()
    float HealthNormalized = 1.f;

    UPROPERTY()
    float AmmoNormalized = 1.f;

    UPROPERTY()
    bool bTargetVisible = false;

    UPROPERTY()
    bool bUpsideDown = false;

    UPROPERTY()
    FString CurrentBranchDebug = TEXT("None");	
};
