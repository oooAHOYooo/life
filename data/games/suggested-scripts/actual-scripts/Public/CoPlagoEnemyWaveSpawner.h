// Copyright Epic Games, Inc. All Rights Reserved.
// CoPlago Enemy Wave Spawner – Zelda-style: spawn one wave at a time; when all enemies in the wave are dead, spawn the next.
// Place in level, set waves (count + enemy class) and spawn points.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoPlagoEnemyWaveSpawner.generated.h"

class ACoPlagoEnemy;

USTRUCT(BlueprintType)
struct FCoPlagoWave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoPlago")
	int32 Count = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoPlago")
	TSubclassOf<ACoPlagoEnemy> EnemyClass;
};

UCLASS()
class CoPlago_API ACoPlagoEnemyWaveSpawner : public AActor
{
	GENERATED_BODY()

public:
	ACoPlagoEnemyWaveSpawner();

	/** Waves: one after the other. When all enemies in wave N are dead, wave N+1 spawns. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoPlago")
	TArray<FCoPlagoWave> Waves;

	/** Spawn locations (world). Use same count as max enemies per wave or more; spawner cycles through. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoPlago")
	TArray<FVector> SpawnLocations;

	/** If no SpawnLocations, use this offset from spawner actor for each enemy (wave 0 at GetActorLocation(), 1 at +Offset, etc.). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoPlago")
	FVector FallbackSpawnOffset = FVector(200.f, 0.f, 0.f);

	/** Called when an enemy in the current wave dies. Do not call manually. */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void NotifyEnemyDied(ACoPlagoEnemy* Enemy);

	/** If true, wave 0 spawns in BeginPlay. If false, call StartWaves() when player enters room (Blueprint). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoPlago")
	bool bStartWavesOnBeginPlay = true;

	/** Start spawning: spawns wave 0. Call from BeginPlay or when player enters room. */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void StartWaves();

	virtual void BeginPlay() override;

	/** Current wave index (0-based). */
	UPROPERTY(BlueprintReadOnly, Category = "CoPlago")
	int32 CurrentWaveIndex = 0;

	/** All waves completed. */
	UFUNCTION(BlueprintNativeEvent, Category = "CoPlago")
	void OnAllWavesComplete();
	virtual void OnAllWavesComplete_Implementation();

protected:
	UPROPERTY()
	TArray<TWeakObjectPtr<ACoPlagoEnemy>> CurrentWaveEnemies;

	void SpawnNextWave();
	FVector GetSpawnLocation(int32 Index) const;
};
