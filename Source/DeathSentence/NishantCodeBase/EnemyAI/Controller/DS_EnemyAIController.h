// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DS_EnemyAIController.generated.h"

/**
 * 
 */

class UDS_EnemyBlackBoard;
class UDSBT_RuntimeContext;
class UDSBT_Tree;
class UDS_TargettingStrategy;

UCLASS()
class DEATHSENTENCE_API ADS_EnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
    ADS_EnemyAIController();
    virtual void Tick(float DeltaSeconds) override;
    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION(BlueprintCallable)
    UDS_TargettingStrategy* GetThreatStrategy() const { return ThreatStrategy; }

    UFUNCTION(BlueprintCallable)
    void SetThreatStrategy(UDS_TargettingStrategy* NewStrat) { ThreatStrategy = NewStrat; }

    UFUNCTION(BlueprintCallable)
    UDS_EnemyBlackBoard* GetBlackboardObject() const { return BlackboardObject; }

protected:
    UPROPERTY()
    TObjectPtr<UDS_EnemyBlackBoard> BlackboardObject;

    UPROPERTY()
    TObjectPtr<UDSBT_RuntimeContext> RuntimeContext;

    UPROPERTY()
    TObjectPtr<UDSBT_Tree> BehaviorTree;

    UPROPERTY(EditAnywhere, Category = "AI")
    TSubclassOf<UDS_TargettingStrategy> ThreatStrategyClass;

    UPROPERTY(EditAnywhere, Category = "AI")
    TObjectPtr<UDS_TargettingStrategy> ThreatStrategy;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackDesiredRange = 1400.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackOrbitRadius = 1400.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackSideOffset = 400.f;

private:
    void RefreshBlackboard();
    void BuildBehaviorTree();
	
};
