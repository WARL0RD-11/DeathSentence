// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/BT/DSBT_Tree.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_Node.h"

void UDSBT_Tree::InitializeTree(UDSBT_RuntimeContext* InContext)
{
	RuntimeContext = InContext;
	if (RootNode)
	{
		RootNode->Initialize(InContext);
	}
}

EDSBT_Status UDSBT_Tree::TickTree(float DeltaTime)
{
	return RootNode ? RootNode->TickNode(DeltaTime) : EDSBT_Status::Failure;
}

void UDSBT_Tree::SetRoot(UDSBT_Node* Root)
{
	RootNode = Root;
}
