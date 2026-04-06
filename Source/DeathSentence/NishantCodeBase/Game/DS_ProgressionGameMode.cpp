#include "NishantCodeBase/Game/DS_ProgressionGameMode.h"
#include "NishantCodeBase/Player/DS_Car.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

ADS_ProgressionGameMode::ADS_ProgressionGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADS_ProgressionGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			100,
			3.0f,
			FColor::Green,
			FString::Printf(TEXT("Level Started: %s"), *CurrentLevelName.ToString()));
	}
}

void ADS_ProgressionGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShowEnemyCountDebug && GEngine)
	{
		const int32 AliveEnemies = GetAliveEnemyCount();

		GEngine->AddOnScreenDebugMessage(
			1,
			0.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Alive Enemies: %d"), AliveEnemies));
	}

	EvaluateRound();
}

ADS_Car* ADS_ProgressionGameMode::FindPlayerVehicle() const
{
	return Cast<ADS_Car>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

int32 ADS_ProgressionGameMode::GetAliveEnemyCount() const
{
	TArray<AActor*> FoundVehicles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADS_Car::StaticClass(), FoundVehicles);

	ADS_Car* PlayerVehicle = FindPlayerVehicle();
	int32 AliveEnemies = 0;

	for (AActor* Actor : FoundVehicles)
	{
		ADS_Car* Vehicle = Cast<ADS_Car>(Actor);
		if (!Vehicle)
		{
			continue;
		}

		if (Vehicle == PlayerVehicle)
		{
			continue;
		}

		if (Vehicle->IsDead())
		{
			continue;
		}

		++AliveEnemies;
	}

	return AliveEnemies;
}

void ADS_ProgressionGameMode::EvaluateRound()
{
	if (bQueuedTransition)
	{
		return;
	}

	ADS_Car* PlayerVehicle = FindPlayerVehicle();

	// Lose condition
	if (!PlayerVehicle || PlayerVehicle->IsDead())
	{
		bPlayerLost = true;
		bPlayerWon = false;
		bQueuedTransition = true;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				2,
				EndRoundDelay,
				FColor::Red,
				TEXT("YOU LOST - Restarting Level"));
		}

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			this,
			&ADS_ProgressionGameMode::RestartLevel,
			EndRoundDelay,
			false);

		return;
	}

	// Win condition
	if (GetAliveEnemyCount() <= 0)
	{
		bPlayerWon = true;
		bPlayerLost = false;
		bQueuedTransition = true;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				3,
				EndRoundDelay,
				FColor::Green,
				FString::Printf(TEXT("YOU WON - Loading %s"), *NextLevelName.ToString()));
		}

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			this,
			&ADS_ProgressionGameMode::GoToNextLevel,
			EndRoundDelay,
			false);
	}
}

void ADS_ProgressionGameMode::GoToNextLevel()
{
	if (NextLevelName.IsNone())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				4,
				3.0f,
				FColor::Red,
				TEXT("NextLevelName is None"));
		}
		return;
	}

	UGameplayStatics::OpenLevel(this, NextLevelName);
}

void ADS_ProgressionGameMode::RestartLevel()
{
	if (CurrentLevelName.IsNone())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				5,
				3.0f,
				FColor::Red,
				TEXT("CurrentLevelName is None"));
		}
		return;
	}

	UGameplayStatics::OpenLevel(this, CurrentLevelName);
}