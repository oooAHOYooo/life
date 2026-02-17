// Copyright Epic Games, Inc. All Rights Reserved.

#include "P2G4WEnemyWaveSpawner.h"
#include "P2G4WEnemy.h"
#include "P2G4W.h"
#include "Kismet/GameplayStatics.h"

AP2G4WEnemyWaveSpawner::AP2G4WEnemyWaveSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AP2G4WEnemyWaveSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (bStartWavesOnBeginPlay && Waves.Num() > 0)
		StartWaves();
}

void AP2G4WEnemyWaveSpawner::StartWaves()
{
	CurrentWaveIndex = 0;
	CurrentWaveEnemies.Empty();
	SpawnNextWave();
}

void AP2G4WEnemyWaveSpawner::NotifyEnemyDied(AP2G4WEnemy* Enemy)
{
	if (!Enemy) return;
	CurrentWaveEnemies.RemoveAll([Enemy](const TWeakObjectPtr<AP2G4WEnemy>& P) { return P.Get() == Enemy; });

	if (CurrentWaveEnemies.Num() > 0) return;

	CurrentWaveIndex++;
	if (CurrentWaveIndex >= Waves.Num())
	{
		OnAllWavesComplete();
		return;
	}
	SpawnNextWave();
}

void AP2G4WEnemyWaveSpawner::SpawnNextWave()
{
	if (CurrentWaveIndex < 0 || CurrentWaveIndex >= Waves.Num()) return;

	const FP2G4WWave& Wave = Waves[CurrentWaveIndex];
	if (!Wave.EnemyClass || Wave.Count <= 0) return;

	UWorld* World = GetWorld();
	if (!World) return;

	CurrentWaveEnemies.Empty();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < Wave.Count; i++)
	{
		FVector Loc = GetSpawnLocation(i);
		AP2G4WEnemy* Enemy = World->SpawnActor<AP2G4WEnemy>(Wave.EnemyClass, Loc, FRotator::ZeroRotator, SpawnParams);
		if (Enemy)
		{
			Enemy->SetWaveSpawner(this);
			CurrentWaveEnemies.Add(Enemy);
		}
	}
}

FVector AP2G4WEnemyWaveSpawner::GetSpawnLocation(int32 Index) const
{
	if (SpawnLocations.IsValidIndex(Index))
		return SpawnLocations[Index];
	if (SpawnLocations.Num() > 0)
		return SpawnLocations[Index % SpawnLocations.Num()];
	return GetActorLocation() + FallbackSpawnOffset * (float)Index;
}

void AP2G4WEnemyWaveSpawner::OnAllWavesComplete_Implementation()
{
}
