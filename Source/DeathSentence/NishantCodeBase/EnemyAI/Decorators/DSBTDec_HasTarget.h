// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_Decorator.h"
#include "DSBTDec_HasTarget.generated.h"

/**
 * 
 */
class UDSBT_RuntimeContext;

UCLASS(EditInlineNew, DefaultToInstanced)
class DEATHSENTENCE_API UDSBTDec_HasTarget : public UDSBT_Decorator
{
	GENERATED_BODY()

public:	
	virtual void Initialize(UDSBT_RuntimeContext* InContext) override;
	virtual EDSBT_Status TickNode(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<UDSBT_RuntimeContext> Context;
	
};
