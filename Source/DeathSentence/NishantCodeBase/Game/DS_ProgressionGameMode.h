#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DS_ProgressionGameMode.generated.h"

class ADS_Car;

UCLASS()
class DEATHSENTENCE_API ADS_ProgressionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADS_ProgressionGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	int32 GetAliveEnemyCount() const;

	UFUNCTION(BlueprintCallable)
	bool HasPlayerWon() const { return bPlayerWon; }

	UFUNCTION(BlueprintCallable)
	bool HasPlayerLost() const { return bPlayerLost; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression")
	FName CurrentLevelName = TEXT("PCG_CrossfireArena");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression")
	FName NextLevelName = TEXT("PCG_MazerunArena");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression")
	float EndRoundDelay = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bShowEnemyCountDebug = true;

private:
	bool bPlayerWon = false;
	bool bPlayerLost = false;
	bool bQueuedTransition = false;

	void EvaluateRound();
	void GoToNextLevel();
	void RestartLevel();

	ADS_Car* FindPlayerVehicle() const;
};