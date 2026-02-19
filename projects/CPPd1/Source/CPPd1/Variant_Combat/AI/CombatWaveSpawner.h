// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "GameFramework/Actor.h"
#include "CombatWaveSpawner.generated.h"

class ACombatEnemy;
class USceneComponent;
class UCapsuleComponent;

/**
 * Wave configuration structure
 */
USTRUCT(BlueprintType)
struct FCombatWaveConfig
{
	GENERATED_BODY()

	/** Number of enemies in this wave */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave", meta = (ClampMin = 1, ClampMax = 20))
	int32 EnemyCount = 3;

	/** Time delay before starting this wave */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave", meta = (ClampMin = 0.0f, Units = "s"))
	float WaveStartDelay = 2.0f;

	/** Time between enemy spawns within the wave. Rush: set to 0 for instant spawn burst. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave", meta = (ClampMin = 0.0f, Units = "s"))
	float SpawnInterval = 1.0f;

	/** Type of enemy to spawn (used if EnemyClassPool is empty) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TSubclassOf<ACombatEnemy> EnemyClass;

	/** If non-empty, each spawn picks a random class from this pool (mixed wave). Otherwise use EnemyClass. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave|Fun")
	TArray<TSubclassOf<ACombatEnemy>> EnemyClassPool;

	/** Intensity scale for this wave. >1 = enemies deal more damage, take less, move faster. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave|Fun", meta = (ClampMin = 0.1f, ClampMax = 5.0f))
	float IntensityScale = 1.0f;

	/** If true, spawn all enemies in this wave at once (ignores SpawnInterval for a rush). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave|Fun")
	bool bRushWave = false;
};

/**
 * Wave-based enemy spawner that spawns enemies in waves (3, 5, etc.)
 * Enemies spawn but only one engages at a time (managed by CombatEngagementManager)
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API ACombatWaveSpawner : public AActor
{
	GENERATED_BODY()

public:
	ACombatWaveSpawner();

protected:
	/** Root scene component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> RootSceneComponent;

	/** Spawn point capsule (visual reference) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> SpawnCapsule;

	/** Array of wave configurations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	TArray<FCombatWaveConfig> WaveConfigs;

	/** If true, start spawning waves immediately on BeginPlay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	bool bStartWavesOnBeginPlay = true;

	/** Current wave index */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	int32 CurrentWaveIndex = 0;

	/** Enemies spawned in current wave (waiting to engage) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	TArray<TObjectPtr<ACombatEnemy>> CurrentWaveEnemies;

	/** All enemies spawned by this spawner */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	TArray<TObjectPtr<ACombatEnemy>> AllSpawnedEnemies;

	/** Timer handles */
	FTimerHandle WaveStartTimer;
	FTimerHandle SpawnTimer;

	/** Current spawn index within wave */
	int32 CurrentSpawnIndex = 0;

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Start spawning waves */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	void StartWaves();

	/** Spawn the next wave */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	void SpawnNextWave();

	/** Spawn a single enemy in the current wave */
	void SpawnEnemyInWave();

	/** Override to change where the next enemy spawns (e.g. spinning radius). Default: SpawnCapsule location. */
	UFUNCTION(BlueprintNativeEvent, Category = "Waves")
	FVector GetSpawnLocation();
	virtual FVector GetSpawnLocation_Implementation();

	/** Called after an enemy is spawned at GetSpawnLocation (e.g. to advance a spinning angle). */
	UFUNCTION(BlueprintNativeEvent, Category = "Waves")
	void OnSpawnLocationUsed();
	virtual void OnSpawnLocationUsed_Implementation();

	/** Called when an enemy dies */
	UFUNCTION()
	void OnEnemyDied(ACombatEnemy* DeadEnemy);

	/** Get all enemies in current wave */
	UFUNCTION(BlueprintPure, Category = "Waves")
	TArray<ACombatEnemy*> GetCurrentWaveEnemies() const;

	/** Get all spawned enemies */
	UFUNCTION(BlueprintPure, Category = "Waves")
	TArray<ACombatEnemy*> GetAllSpawnedEnemies() const;

	/** Get current wave index (0-based) */
	UFUNCTION(BlueprintPure, Category = "Waves")
	int32 GetCurrentWaveIndex() const { return CurrentWaveIndex; }

	/** Check if all waves are complete */
	UFUNCTION(BlueprintPure, Category = "Waves")
	bool AreAllWavesComplete() const;

	/** Reset spawner to start from first wave */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	void ResetWaves();

protected:
	/** Delegate for when a wave starts */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStarted, int32, WaveIndex);

	/** Delegate for when a wave completes */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveCompleted, int32, WaveIndex);

	/** Delegate for when all waves complete */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllWavesCompleted);

	/** Delegate for when a single enemy is spawned (so engagement manager can register it) */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawned, ACombatEnemy*, Enemy);

public:
	/** Event fired when a wave starts */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWaveStarted OnWaveStarted;

	/** Event fired when an enemy is spawned in a wave */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemySpawned OnEnemySpawned;

	/** Event fired when a wave completes */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWaveCompleted OnWaveCompleted;

	/** Event fired when all waves complete */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAllWavesCompleted OnAllWavesCompleted;
};
