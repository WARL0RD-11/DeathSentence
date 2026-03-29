// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_Node.h"
#include "DSBTTask_Roam.generated.h"

/**
 * 
 */
class UDSBT_RuntimeContext;

UCLASS(EditInlineNew, DefaultToInstanced)
class DEATHSENTENCE_API UDSBTTask_Roam : public UDSBT_Node
{
	GENERATED_BODY()
public:
	virtual void Initialize(UDSBT_RuntimeContext* InContext) override;
	virtual EDSBT_Status TickNode(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<UDSBT_RuntimeContext> Context;
	
	UPROPERTY(EditAnywhere)
	float RoamRadius = 2500.f;
};
