// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_Node.h"
#include "DSBT_CompositeNode.generated.h"

/**
 * 
 */
UCLASS()
class DEATHSENTENCE_API UDSBT_CompositeNode : public UDSBT_Node
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UDSBT_Node>> Children;

	virtual void Initialize(UDSBT_RuntimeContext* InContext) override;
	
};
