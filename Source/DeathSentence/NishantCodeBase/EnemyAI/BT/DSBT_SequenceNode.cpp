// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/BT/DSBT_SequenceNode.h"

EDSBT_Status UDSBT_SequenceNode::TickNode(float DeltaTime)
{
	for (; ActiveChild < Children.Num(); ActiveChild++)
	{
		if (!Children[ActiveChild]) continue;

		const EDSBT_Status Result = Children[ActiveChild]->TickNode(DeltaTime);

		if (Result == EDSBT_Status::Running)
		{
			return EDSBT_Status::Running;
		}

		if (Result == EDSBT_Status::Failure)
		{
			ActiveChild = 0;
			return EDSBT_Status::Failure;
		}
	}

	ActiveChild = 0;
	return EDSBT_Status::Success;
}

void UDSBT_SequenceNode::ResetNode()
{
	ActiveChild = 0;
	for (UDSBT_Node* Child : Children)
	{
		if (Child)
		{
			Child->ResetNode();
		}
	}
}
