// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NishantCodeBase/PCG/DS_GridTypes.h"
#include "NishantCodeBase/PCG/DS_LevelConfig.h"
#include "DS_ArenaGeneratorBase.generated.h"

class UDS_ArenaArchetypeStrategy;
class ADS_Car;

UCLASS()
class DEATHSENTENCE_API ADS_ArenaGeneratorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ADS_ArenaGeneratorBase();

	UFUNCTION(BlueprintCallable)
	void GenerateArena();

	UFUNCTION(BlueprintCallable)
	void SetLevelConfig(const FDS_LevelConfig& NewConfig);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	FDS_LevelConfig LevelConfig;

	UPROPERTY(EditAnywhere, Instanced, Category = "Generation")
	TObjectPtr<UDS_ArenaArchetypeStrategy> ArchetypeStrategy;
	
	UPROPERTY(EditAnywhere, Category = "Generation|Spawn")
	TSubclassOf<AActor> FloorTileClass;
	
	UPROPERTY(EditAnywhere, Category = "Generation|Spawn")
	TSubclassOf<AActor> WallClass;
	
	UPROPERTY(EditAnywhere, Category = "Generation|Spawn")
	TSubclassOf<AActor> CoverClass;
	
	UPROPERTY(EditAnywhere, Category = "Generation|Spawn")
	TSubclassOf<AActor> CenterpieceClass;
	
	UPROPERTY(EditAnywhere, Category = "Generation|Spawn")
	TSubclassOf<ADS_Car> EnemyAggressorClass;
	
	UPROPERTY(EditAnywhere, Category = "Generation|Spawn")
	TSubclassOf<ADS_Car> EnemyDefenderClass;
	
	UPROPERTY(EditAnywhere, Category = "Generation|Spawn")
	TSubclassOf<AActor> HealthPickupClass;
	
	UPROPERTY(EditAnywhere, Category = "Generation|Spawn")
	TSubclassOf<AActor> AmmoPickupClass;
	
	UPROPERTY(EditAnywhere, Category = "Generation|Spawn")
	TSubclassOf<AActor> BoostPickupClass;
	
	UPROPERTY(EditAnywhere, Category = "Generation|Spawn")
	TSubclassOf<AActor> MinigunPickupClass;
	
	UPROPERTY(EditAnywhere, Category = "Generation")
	FVector ArenaOrigin = FVector::ZeroVector;

protected:
	virtual void BeginPlay() override;

	virtual void ResetGenerationState();
	virtual void BuildGrid();
	void ReservePlayerSpawnZones();
	void ReserveEnemySpawnZones();

	bool IsOpenNavigableCell(const FDS_GridCell& Cell) const;
	bool CanUseCellForSpawn(int32 X, int32 Y, int32 ClearanceRadius) const;
	bool IsFarEnoughFromChosenCells(const FIntPoint& Candidate, const TArray<FIntPoint>& ChosenCells, float MinDistanceInCells) const;
	void ShuffleCells(TArray<FIntPoint>& Cells);

	const FDS_GridCell* GetCellConst(int32 X, int32 Y) const;
	virtual void ApplyArchetypeMainStructure();
	virtual void ApplyArchetypeLanePass();
	virtual void ApplyArchetypeCoverPass();
	virtual void PlaceRiskRewardPickupCells();
	virtual void MaterializeArena();
	FDS_GridCell* GetCell(int32 X, int32 Y);
	FVector CellToWorld(const FIntPoint& Coord, float Z = 0.f) const;

private:	
	UPROPERTY()
	TArray<FDS_GridCell> Grid;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;

	FRandomStream RNG;

};
