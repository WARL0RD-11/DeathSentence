// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/BT/DSBT_RuntimeContext.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/EnemyAI/Controller/DS_EnemyAIController.h"
#include "NishantCodeBase/Player/DS_Car.h"

void UDSBT_RuntimeContext::Setup(ADS_EnemyAIController* InController, ADS_Car* InVehicle, UDS_EnemyBlackBoard* InBoard)
{
	Blackboard = InBoard;
	mController = InController;
	SelfVehicle = InVehicle;
}
