// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/BT/DSBT_SelectorNode.h"

EDSBT_Status UDSBT_SelectorNode::TickNode(float DeltaTime)
{
	int32 NewRunningChildIndex = INDEX_NONE;

	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		UDSBT_Node* Child = Children[ChildIndex];
		if (!Child)
		{
			continue;
		}

		const EDSBT_Status Result = Child->TickNode(DeltaTime);

		if (Result == EDSBT_Status::Running)
		{
			NewRunningChildIndex = ChildIndex;

			if (LastRunningChildIndex != INDEX_NONE && LastRunningChildIndex != NewRunningChildIndex)
			{
				if (Children.IsValidIndex(LastRunningChildIndex) && Children[LastRunningChildIndex])
				{
					Children[LastRunningChildIndex]->ResetNode();
				}
			}

			LastRunningChildIndex = NewRunningChildIndex;
			return EDSBT_Status::Running;
		}

		if (Result == EDSBT_Status::Success)
		{
			if (LastRunningChildIndex != INDEX_NONE && LastRunningChildIndex != ChildIndex)
			{
				if (Children.IsValidIndex(LastRunningChildIndex) && Children[LastRunningChildIndex])
				{
					Children[LastRunningChildIndex]->ResetNode();
				}
			}

			LastRunningChildIndex = INDEX_NONE;
			return EDSBT_Status::Success;
		}
	}

	if (LastRunningChildIndex != INDEX_NONE)
	{
		if (Children.IsValidIndex(LastRunningChildIndex) && Children[LastRunningChildIndex])
		{
			Children[LastRunningChildIndex]->ResetNode();
		}
		LastRunningChildIndex = INDEX_NONE;
	}

	return EDSBT_Status::Failure;
}

void UDSBT_SelectorNode::ResetNode()
{
	LastRunningChildIndex = INDEX_NONE;

	for (UDSBT_Node* Child : Children)
	{
		if (Child)
		{
			Child->ResetNode();
		}
	}
}
