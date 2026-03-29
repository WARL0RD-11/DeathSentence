// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_Node.h"
#include "DSBTTask_FindTarget.generated.h"

/**
 * 
 */

class UDSBT_RuntimeContext;

UCLASS(EditInlineNew, DefaultToInstanced)
class DEATHSENTENCE_API UDSBTTask_FindTarget : public UDSBT_Node
{
	GENERATED_BODY()
public:
	virtual void Initialize(UDSBT_RuntimeContext* InContext) override;
	virtual EDSBT_Status TickNode(float DeltaTime) override;
	virtual void ResetNode() override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float RetargetInterval = 2.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float CurrentTargetScorePenalty = 600.f;

private: 
	UPROPERTY()
	TObjectPtr<UDSBT_RuntimeContext> Context;
	float TimeUntilRetarget = 0.f;
};
