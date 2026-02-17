// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoPlagoEnemyWaveSpawner.h"
#include "CoPlagoEnemy.h"
#include "CoPlago.h"
#include "Kismet/GameplayStatics.h"

ACoPlagoEnemyWaveSpawner::ACoPlagoEnemyWaveSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACoPlagoEnemyWaveSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (bStartWavesOnBeginPlay && Waves.Num() > 0)
		StartWaves();
}

void ACoPlagoEnemyWaveSpawner::StartWaves()
{
	CurrentWaveIndex = 0;
	CurrentWaveEnemies.Empty();
	SpawnNextWave();
}

void ACoPlagoEnemyWaveSpawner::NotifyEnemyDied(ACoPlagoEnemy* Enemy)
{
	if (!Enemy) return;
	CurrentWaveEnemies.RemoveAll([Enemy](const TWeakObjectPtr<ACoPlagoEnemy>& P) { return P.Get() == Enemy; });

	if (CurrentWaveEnemies.Num() > 0) return;

	CurrentWaveIndex++;
	if (CurrentWaveIndex >= Waves.Num())
	{
		OnAllWavesComplete();
		return;
	}
	SpawnNextWave();
}

void ACoPlagoEnemyWaveSpawner::SpawnNextWave()
{
	if (CurrentWaveIndex < 0 || CurrentWaveIndex >= Waves.Num()) return;

	const FCoPlagoWave& Wave = Waves[CurrentWaveIndex];
	if (!Wave.EnemyClass || Wave.Count <= 0) return;

	UWorld* World = GetWorld();
	if (!World) return;

	CurrentWaveEnemies.Empty();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < Wave.Count; i++)
	{
		FVector Loc = GetSpawnLocation(i);
		ACoPlagoEnemy* Enemy = World->SpawnActor<ACoPlagoEnemy>(Wave.EnemyClass, Loc, FRotator::ZeroRotator, SpawnParams);
		if (Enemy)
		{
			Enemy->SetWaveSpawner(this);
			CurrentWaveEnemies.Add(Enemy);
		}
	}
}

FVector ACoPlagoEnemyWaveSpawner::GetSpawnLocation(int32 Index) const
{
	if (SpawnLocations.IsValidIndex(Index))
		return SpawnLocations[Index];
	if (SpawnLocations.Num() > 0)
		return SpawnLocations[Index % SpawnLocations.Num()];
	return GetActorLocation() + FallbackSpawnOffset * (float)Index;
}

void ACoPlagoEnemyWaveSpawner::OnAllWavesComplete_Implementation()
{
}
