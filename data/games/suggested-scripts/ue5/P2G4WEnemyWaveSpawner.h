// Copyright Epic Games, Inc. All Rights Reserved.
// P2G4W Enemy Wave Spawner – Zelda-style: spawn one wave at a time; when all enemies in the wave are dead, spawn the next.
// Place in level, set waves (count + enemy class) and spawn points.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P2G4WEnemyWaveSpawner.generated.h"

class AP2G4WEnemy;

USTRUCT(BlueprintType)
struct FP2G4WWave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P2G4W")
	int32 Count = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P2G4W")
	TSubclassOf<AP2G4WEnemy> EnemyClass;
};

UCLASS()
class P2G4W_API AP2G4WEnemyWaveSpawner : public AActor
{
	GENERATED_BODY()

public:
	AP2G4WEnemyWaveSpawner();

	/** Waves: one after the other. When all enemies in wave N are dead, wave N+1 spawns. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P2G4W")
	TArray<FP2G4WWave> Waves;

	/** Spawn locations (world). Use same count as max enemies per wave or more; spawner cycles through. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P2G4W")
	TArray<FVector> SpawnLocations;

	/** If no SpawnLocations, use this offset from spawner actor for each enemy (wave 0 at GetActorLocation(), 1 at +Offset, etc.). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P2G4W")
	FVector FallbackSpawnOffset = FVector(200.f, 0.f, 0.f);

	/** Called when an enemy in the current wave dies. Do not call manually. */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	void NotifyEnemyDied(AP2G4WEnemy* Enemy);

	/** If true, wave 0 spawns in BeginPlay. If false, call StartWaves() when player enters room (Blueprint). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P2G4W")
	bool bStartWavesOnBeginPlay = true;

	/** Start spawning: spawns wave 0. Call from BeginPlay or when player enters room. */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	void StartWaves();

	virtual void BeginPlay() override;

	/** Current wave index (0-based). */
	UPROPERTY(BlueprintReadOnly, Category = "P2G4W")
	int32 CurrentWaveIndex = 0;

	/** All waves completed. */
	UFUNCTION(BlueprintNativeEvent, Category = "P2G4W")
	void OnAllWavesComplete();
	virtual void OnAllWavesComplete_Implementation();

protected:
	UPROPERTY()
	TArray<TWeakObjectPtr<AP2G4WEnemy>> CurrentWaveEnemies;

	void SpawnNextWave();
	FVector GetSpawnLocation(int32 Index) const;
};
