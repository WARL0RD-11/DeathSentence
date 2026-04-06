// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NishantCodeBase/PCG/DS_LevelConfig.h"	
#include "DS_ArenaDirector.generated.h"

class ADS_ArenaGeneratorBase;	
class UDS_ArenaCrossfire;
class UDS_ArenaMazeRun;

UCLASS()
class DEATHSENTENCE_API ADS_ArenaDirector : public AActor
{
	GENERATED_BODY()
	
public:	

	UFUNCTION(BlueprintCallable)
	void BuildLevel(int32 LevelIndex);

	UPROPERTY(EditAnywhere)
	TObjectPtr<ADS_ArenaGeneratorBase> ArenaGenerator;

protected:
	virtual void BeginPlay() override;

};
