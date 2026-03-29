// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DSBT_RuntimeContext.generated.h"

/**
 * 
 */

class ADS_EnemyAIController;
class ADS_Car;
class UDS_EnemyBlackBoard;

UCLASS()
class DEATHSENTENCE_API UDSBT_RuntimeContext : public UObject
{
	GENERATED_BODY()

public:
    UPROPERTY()
    TObjectPtr<ADS_EnemyAIController> mController;

    UPROPERTY()
    TObjectPtr<ADS_Car> SelfVehicle;

    UPROPERTY()
    TObjectPtr<UDS_EnemyBlackBoard> Blackboard;

    void Setup(ADS_EnemyAIController* InController, ADS_Car* InVehicle, UDS_EnemyBlackBoard* InBoard);

};
