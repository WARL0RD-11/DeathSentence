// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DSBT_Node.generated.h"

/**
 * 
 */

class UDSBT_RuntimeContext;


UENUM()
enum class EDSBT_Status : uint8
{
	Success,
	Failure,
	Running
};

UCLASS()
class DEATHSENTENCE_API UDSBT_Node : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(UDSBT_RuntimeContext* InContext) {}
	virtual EDSBT_Status TickNode(float DeltaTime) PURE_VIRTUAL(UDSBT_Node::TickNode, return EDSBT_Status::Failure;);
	virtual void ResetNode() {}
	virtual FString GetDebugName() const { return GetClass()->GetName(); }
};
