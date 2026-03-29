// Fill out your copyright notice in the Description page of Project Settings.


#include "NishantCodeBase/EnemyAI/Controller/DS_EnemyAIController.h"
#include "NishantCodeBase/EnemyAI/BlackBoard/DS_EnemyBlackBoard.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_RuntimeContext.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_Tree.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_SelectorNode.h"
#include "NishantCodeBase/EnemyAI/BT/DSBT_SequenceNode.h"
#include "NishantCodeBase/EnemyAI/Strategy/DS_TargettingStrategy.h"
#include "NishantCodeBase/EnemyAI/Strategy/DS_PickupStrategy.h"
#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_DriveToTarget.h"
#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_FindTarget.h"
#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_FireWeapon.h"
#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_Roam.h"
#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_FindBestPickup.h"
#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_DirveToPickup.h"
#include "NishantCodeBase/EnemyAI/Tasks/DSBTTask_Strafe.h"
#include "NishantCodeBase/EnemyAI/Decorators/DSBTDec_LowAmmo.h"
#include "NishantCodeBase/EnemyAI/Decorators/DSBTDec_HasAmmo.h"
#include "NishantCodeBase/EnemyAI/Decorators/DSBTDec_LowHealth.h"
#include "NishantCodeBase/Components/HealthComponent/DS_HealthComponent.h"
#include "NishantCodeBase/Components/WeaponComponent/WeaponComponent.h"
#include "NishantCodeBase/Player/DS_Car.h"
#include "Engine/Engine.h"

ADS_EnemyAIController::ADS_EnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	ThreatStrategyClass = UDS_AggressiveTargetting::StaticClass();
}

void ADS_EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ADS_Car* Vehicle = Cast<ADS_Car>(InPawn);
	if (!Vehicle) return;

	BlackboardObject = NewObject<UDS_EnemyBlackBoard>(this);
	BlackboardObject->SelfCar = Vehicle;

	RuntimeContext = NewObject<UDSBT_RuntimeContext>(this);
	RuntimeContext->Setup(this, Vehicle, BlackboardObject.Get());
	if (ThreatStrategyClass)
	{
		if (ThreatStrategyClass->HasAnyClassFlags(CLASS_Abstract))
		{
			UE_LOG(LogTemp, Error, TEXT("ThreatStrategyClass is abstract on %s"), *GetName());
			return;
		}

		ThreatStrategy = NewObject<UDS_TargettingStrategy>(this, ThreatStrategyClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ThreatStrategyClass is null on %s"), *GetName());
	}

	BuildBehaviorTree();
	if (BehaviorTree)
	{
		BehaviorTree->InitializeTree(RuntimeContext);
	}
}

void ADS_EnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APawn* ControlledPawn = RuntimeContext && RuntimeContext->mController ? RuntimeContext->mController->GetPawn() : nullptr;

	ADS_Car* Car = Cast<ADS_Car>(ControlledPawn);
	if (Car)
	{
		GEngine->AddOnScreenDebugMessage(
			(int32)((PTRINT)this & 0x7FFFFFFF),
			0.f,
			FColor::Red,
			TEXT("CAST OKAYY")
		);
	}

	RefreshBlackboard();

	if (BehaviorTree)
	{
		BehaviorTree->TickTree(DeltaSeconds);
	}

	if (BlackboardObject && GEngine)
	{
		const FString StrategyName = ThreatStrategy ? ThreatStrategy->GetClass()->GetName() : TEXT("None");

		GEngine->AddOnScreenDebugMessage(
			(int32)((PTRINT)this & 0x7FFFFFFF),
			0.f,
			FColor::Green,
			FString::Printf(TEXT("%s | Strategy=%s | Branch=%s | HP=%.2f | Ammo=%.2f | Range=%.0f"),
				*GetName(),
				*StrategyName,
				*BlackboardObject->CurrentBranchDebug,
				BlackboardObject->HealthNormalized,
				BlackboardObject->AmmoNormalized,
				AttackDesiredRange));
	}
}

void ADS_EnemyAIController::RefreshBlackboard()
{
	if (!BlackboardObject || !BlackboardObject->SelfCar) return;

	if (UDS_HealthComponent* HC = BlackboardObject->SelfCar->GetHealthComponent())
	{
		BlackboardObject->HealthNormalized = HC->GetNormalizedHealth();
	}

	if (UWeaponComponent* WC = BlackboardObject->SelfCar->GetWeaponComponent())
	{
		BlackboardObject->AmmoNormalized = WC->GetCannonAmmoNormalized();
	}

	BlackboardObject->bUpsideDown =
		FVector::DotProduct(BlackboardObject->SelfCar->GetActorUpVector(), FVector::UpVector) < 0.f;
}

void ADS_EnemyAIController::BuildBehaviorTree()
{
	BehaviorTree = NewObject<UDSBT_Tree>(this);

	UDSBT_SelectorNode* Root = NewObject<UDSBT_SelectorNode>(BehaviorTree);

	UDSBTDec_LowHealth* RepairBranch = NewObject<UDSBTDec_LowHealth>(Root);
	{
		UDSBT_SequenceNode* RepairSequence = NewObject<UDSBT_SequenceNode>(RepairBranch);
		UDSBTTask_FindBestPickup* FindPickup = NewObject<UDSBTTask_FindBestPickup>(RepairSequence);
		FindPickup->PickupStrat = NewObject<UDS_Pickup_RepairPriority>(FindPickup);

		RepairSequence->Children.Add(FindPickup);
		RepairSequence->Children.Add(NewObject<UDSBTTask_DirveToPickup>(RepairSequence));
		RepairBranch->Child = RepairSequence;
	}

	UDSBTDec_HasAmmo* AttackBranch = NewObject<UDSBTDec_HasAmmo>(Root);
	{
		UDSBT_SequenceNode* AttackSequence = NewObject<UDSBT_SequenceNode>(AttackBranch);
		AttackSequence->Children.Add(NewObject<UDSBTTask_FindTarget>(AttackSequence));
		UDSBTTask_DriveToTarget* DriveTask = NewObject<UDSBTTask_DriveToTarget>(AttackSequence);
		DriveTask->DesiredRange = AttackDesiredRange;
		AttackSequence->Children.Add(DriveTask);

		AttackSequence->Children.Add(NewObject<UDSBTTask_FireWeapon>(AttackSequence));

		UDSBTTask_Strafe* Strafe = NewObject<UDSBTTask_Strafe>(AttackSequence);
		Strafe->OrbitRadius = AttackOrbitRadius;
		Strafe->SideOffset = AttackSideOffset;
		AttackSequence->Children.Add(Strafe);
		AttackBranch->Child = AttackSequence;
	}

	UDSBTDec_LowAmmo* ReloadBranch = NewObject<UDSBTDec_LowAmmo>(Root);
	{
		UDSBT_SequenceNode* ReloadSequence = NewObject<UDSBT_SequenceNode>(ReloadBranch);
		UDSBTTask_FindBestPickup* FindPickup = NewObject<UDSBTTask_FindBestPickup>(ReloadSequence);
		FindPickup->PickupStrat = NewObject<UDS_Pickup_ReloadPriority>(FindPickup);

		ReloadSequence->Children.Add(FindPickup);
		ReloadSequence->Children.Add(NewObject<UDSBTTask_DirveToPickup>(ReloadSequence));
		ReloadBranch->Child = ReloadSequence;
	}

	UDSBTTask_Roam* RoamingBranch = NewObject<UDSBTTask_Roam>(Root);
	
	Root->Children.Add(RepairBranch);
	Root->Children.Add(AttackBranch);
	Root->Children.Add(ReloadBranch);
	Root->Children.Add(RoamingBranch);


	BehaviorTree->SetRoot(Root);

}
