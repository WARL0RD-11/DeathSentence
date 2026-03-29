// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_CompositeNode.h"
#include "DSBT_SelectorNode.generated.h"

/**
 * 
 */
UCLASS()
class DEATHSENTENCE_API UDSBT_SelectorNode : public UDSBT_CompositeNode
{
	GENERATED_BODY()
	
public:
	virtual EDSBT_Status TickNode(float DeltaTime) override;
	virtual void ResetNode() override;

private:
	int32 LastRunningChildIndex = INDEX_NONE;
};
