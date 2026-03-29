// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Decorators/DSBTDec_LowHealth.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_RuntimeContext.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"

void UDSBTDec_LowHealth::Initialize(UDSBT_RuntimeContext* InContext)
{
	Context = InContext;
	Super::Initialize(InContext);
}

EDSBT_Status UDSBTDec_LowHealth::TickNode(float DeltaTime)
{
	if (!Context || !Context->Blackboard)
	{
		return EDSBT_Status::Failure;
	}
	if (Context->Blackboard->HealthNormalized > HealthThreshold)
	{
		return EDSBT_Status::Failure;
	}

	return Child ? Child->TickNode(DeltaTime) : EDSBT_Status::Failure;
}
