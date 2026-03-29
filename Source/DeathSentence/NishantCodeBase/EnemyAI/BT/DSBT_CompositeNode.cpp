// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/BT/DSBT_CompositeNode.h"

void UDSBT_CompositeNode::Initialize(UDSBT_RuntimeContext* InContext)
{
	for ( UDSBT_Node* Child : Children)
	{
		if (Child)
		{
			Child->Initialize(InContext);
		}
	}
}
