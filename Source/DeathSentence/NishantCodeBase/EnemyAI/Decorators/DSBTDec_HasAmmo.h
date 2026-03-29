// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_Decorator.h"
#include "DSBTDec_HasAmmo.generated.h"

/**
 * 
 */
class UDSBT_RuntimeContext;

UCLASS()
class DEATHSENTENCE_API UDSBTDec_HasAmmo : public UDSBT_Decorator
{
	GENERATED_BODY()
public:
	virtual void Initialize(UDSBT_RuntimeContext* InContext) override;
	virtual EDSBT_Status TickNode(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AmmoThreshold = 0.25f;

private: 
	UPROPERTY()
	TObjectPtr<UDSBT_RuntimeContext> Context;
	
};
