// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_FireWeapon.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_RuntimeContext.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/Components/WeaponComponent/WeaponComponent.h"
#include "NishantCodeBase/Player/DS_Car.h"

void UDSBTTask_FireWeapon::Initialize(UDSBT_RuntimeContext* InContext)
{
	Context = InContext;
}
//TODO add firing delay

EDSBT_Status UDSBTTask_FireWeapon::TickNode(float DeltaTime)
{

	if (!Context || !Context->mController || !Context->Blackboard || !Context->SelfVehicle) return EDSBT_Status::Failure;

	Context->Blackboard->CurrentBranchDebug = TEXT("Firing Weapon");

	if (FireCooldownRemaining > 0.0f)
	{
		FireCooldownRemaining -= DeltaTime;
		return EDSBT_Status::Running;
	}

	if (UWeaponComponent* Weapons = Context->SelfVehicle->GetWeaponComponent())
	{
		Weapons->TryFireCannon(Context->Blackboard->CurrentTarget);
		FireCooldownRemaining = FireInterval;
		return EDSBT_Status::Success;
	}

	return EDSBT_Status::Failure;
}

void UDSBTTask_FireWeapon::ResetNode()
{
	Super::ResetNode();
	FireCooldownRemaining = 0.0f;
}