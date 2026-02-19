// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpinningRadiusWaveSpawner.h"
#include "Variant_Combat/AI/CombatWaveSpawner.h"

ASpinningRadiusWaveSpawner::ASpinningRadiusWaveSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

FVector ASpinningRadiusWaveSpawner::GetSpawnLocation_Implementation()
{
	const float Rad = FMath::DegreesToRadians(CurrentSpawnAngle);
	const FVector Offset(
		SpawnRadius * FMath::Cos(Rad),
		SpawnRadius * FMath::Sin(Rad),
		SpawnHeightOffset
	);
	return GetActorLocation() + Offset;
}

void ASpinningRadiusWaveSpawner::OnSpawnLocationUsed_Implementation()
{
	CurrentSpawnAngle += AngleStepPerSpawn;
	if (CurrentSpawnAngle >= 360.0f) CurrentSpawnAngle -= 360.0f;
	if (CurrentSpawnAngle < 0.0f) CurrentSpawnAngle += 360.0f;
}
