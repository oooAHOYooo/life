// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Combat/AI/CombatWaveSpawner.h"
#include "Variant_Combat/AI/CombatEnemy.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

ACombatWaveSpawner::ACombatWaveSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComponent;

	SpawnCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SpawnCapsule"));
	SpawnCapsule->SetupAttachment(RootComponent);
	SpawnCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	SpawnCapsule->SetCapsuleSize(35.0f, 90.0f);
	SpawnCapsule->SetCollisionProfileName(FName("NoCollision"));
	SpawnCapsule->SetHiddenInGame(true);
}

void ACombatWaveSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bStartWavesOnBeginPlay && WaveConfigs.Num() > 0)
	{
		StartWaves();
	}
}

void ACombatWaveSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(WaveStartTimer);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
}

void ACombatWaveSpawner::StartWaves()
{
	CurrentWaveIndex = 0;
	CurrentWaveEnemies.Empty();
	AllSpawnedEnemies.Empty();

	if (WaveConfigs.Num() > 0)
	{
		SpawnNextWave();
	}
}

void ACombatWaveSpawner::SpawnNextWave()
{
	if (CurrentWaveIndex >= WaveConfigs.Num())
	{
		// All waves complete
		OnAllWavesCompleted.Broadcast();
		return;
	}

	const FCombatWaveConfig& WaveConfig = WaveConfigs[CurrentWaveIndex];
	CurrentSpawnIndex = 0;
	CurrentWaveEnemies.Empty();

	OnWaveStarted.Broadcast(CurrentWaveIndex);

	// Schedule first enemy spawn after delay
	if (WaveConfig.WaveStartDelay > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			WaveStartTimer,
			this,
			&ACombatWaveSpawner::SpawnEnemyInWave,
			WaveConfig.WaveStartDelay,
			false
		);
	}
	else
	{
		SpawnEnemyInWave();
	}
}

void ACombatWaveSpawner::SpawnEnemyInWave()
{
	if (CurrentWaveIndex >= WaveConfigs.Num())
	{
		return;
	}

	const FCombatWaveConfig& WaveConfig = WaveConfigs[CurrentWaveIndex];

	// Check if we've spawned all enemies for this wave
	if (CurrentSpawnIndex >= WaveConfig.EnemyCount)
	{
		// Wave spawning complete, move to next wave when current wave is defeated
		return;
	}

	// Spawn enemy
	if (WaveConfig.EnemyClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector SpawnLocation = GetSpawnLocation();
		FRotator SpawnRotation = SpawnCapsule->GetComponentRotation();

		// Add some random offset to prevent overlapping (if using default spawn location)
		if (SpawnLocation == SpawnCapsule->GetComponentLocation())
		{
			SpawnLocation += FVector(
				FMath::RandRange(-100.0f, 100.0f),
				FMath::RandRange(-100.0f, 100.0f),
				0.0f
			);
		}

		ACombatEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ACombatEnemy>(
			WaveConfig.EnemyClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);

		if (SpawnedEnemy)
		{
			CurrentWaveEnemies.Add(SpawnedEnemy);
			AllSpawnedEnemies.Add(SpawnedEnemy);

			// Subscribe to death event
			SpawnedEnemy->OnEnemyDied.AddDynamic(this, &ACombatWaveSpawner::OnEnemyDied);
		}

		// Notify that spawn location was used
		OnSpawnLocationUsed();
	}

	CurrentSpawnIndex++;

	// Schedule next enemy spawn if there are more in this wave
	if (CurrentSpawnIndex < WaveConfig.EnemyCount)
	{
		GetWorld()->GetTimerManager().SetTimer(
			SpawnTimer,
			this,
			&ACombatWaveSpawner::SpawnEnemyInWave,
			WaveConfig.SpawnInterval,
			false
		);
	}
}

void ACombatWaveSpawner::OnEnemyDied(ACombatEnemy* DeadEnemy)
{
	if (!DeadEnemy)
	{
		return;
	}

	// Remove from current wave
	CurrentWaveEnemies.Remove(DeadEnemy);

	// Check if current wave is complete
	if (CurrentWaveEnemies.Num() == 0)
	{
		OnWaveCompleted.Broadcast(CurrentWaveIndex);
		CurrentWaveIndex++;

		// Start next wave
		if (CurrentWaveIndex < WaveConfigs.Num())
		{
			SpawnNextWave();
		}
		else
		{
			OnAllWavesCompleted.Broadcast();
		}
	}
}

TArray<ACombatEnemy*> ACombatWaveSpawner::GetCurrentWaveEnemies() const
{
	TArray<ACombatEnemy*> Result;
	for (ACombatEnemy* Enemy : CurrentWaveEnemies)
	{
		if (IsValid(Enemy))
		{
			Result.Add(Enemy);
		}
	}
	return Result;
}

TArray<ACombatEnemy*> ACombatWaveSpawner::GetAllSpawnedEnemies() const
{
	TArray<ACombatEnemy*> Result;
	for (ACombatEnemy* Enemy : AllSpawnedEnemies)
	{
		if (IsValid(Enemy))
		{
			Result.Add(Enemy);
		}
	}
	return Result;
}

bool ACombatWaveSpawner::AreAllWavesComplete() const
{
	return CurrentWaveIndex >= WaveConfigs.Num() && CurrentWaveEnemies.Num() == 0;
}

void ACombatWaveSpawner::ResetWaves()
{
	CurrentWaveIndex = 0;
	CurrentSpawnIndex = 0;
	CurrentWaveEnemies.Empty();
	AllSpawnedEnemies.Empty();

	GetWorld()->GetTimerManager().ClearTimer(WaveStartTimer);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
}

FVector ACombatWaveSpawner::GetSpawnLocation_Implementation()
{
	return SpawnCapsule ? SpawnCapsule->GetComponentLocation() : GetActorLocation();
}

void ACombatWaveSpawner::OnSpawnLocationUsed_Implementation()
{
	// Default implementation does nothing
}
