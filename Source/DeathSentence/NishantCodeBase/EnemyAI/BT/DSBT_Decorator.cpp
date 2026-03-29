// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/BT/DSBT_Decorator.h"

void UDSBT_Decorator::Initialize(UDSBT_RuntimeContext* InContext)
{
	if (Child)
		Child->Initialize(InContext);
}

void UDSBT_Decorator::ResetNode()
{
	if (Child)
		Child->ResetNode();
}
