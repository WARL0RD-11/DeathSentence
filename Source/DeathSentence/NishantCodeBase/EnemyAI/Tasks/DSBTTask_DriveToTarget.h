// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_Node.h"
#include "DSBTTask_DriveToTarget.generated.h"

/**
 * 
 */
class UDSBT_RuntimeContext;

UCLASS(EditInlineNew, DefaultToInstanced)
class DEATHSENTENCE_API UDSBTTask_DriveToTarget : public UDSBT_Node
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(UDSBT_RuntimeContext* InContext) override;
	virtual EDSBT_Status TickNode(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DesiredRange = 1400.f;
private:
	UPROPERTY()
	TObjectPtr<UDSBT_RuntimeContext> Context;
};
