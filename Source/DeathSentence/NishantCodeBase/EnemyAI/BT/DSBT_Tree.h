// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DSBT_Tree.generated.h"

/**
 * 
 */

class UDSBT_RuntimeContext;
class UDSBT_Node;
enum class EDSBT_Status : uint8;

UCLASS()
class DEATHSENTENCE_API UDSBT_Tree : public UObject
{
	GENERATED_BODY()

public:
	void InitializeTree(UDSBT_RuntimeContext* InContext);
	EDSBT_Status TickTree(float DeltaTime);
	void SetRoot(UDSBT_Node* Root);

private:
	UPROPERTY()
	TObjectPtr<UDSBT_RuntimeContext> RuntimeContext;

	UPROPERTY()
	TObjectPtr<UDSBT_Node> RootNode;
	
};
