// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "Variant_Combat/AI/CombatWaveSpawner.h"
#include "SpinningRadiusWaveSpawner.generated.h"

/**
 * Wave spawner that spawns enemies on a circle around the actor. Each spawn advances the angle
 * so spawn points "spin" around the radius—great for marauders emerging from a zone.
 * Use Wave Configs and Enemy Class / Enemy Class Pool (e.g. TricksterMarauder, BruteMarauder) as with CombatWaveSpawner.
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API ASpinningRadiusWaveSpawner : public ACombatWaveSpawner
{
	GENERATED_BODY()

public:
	ASpinningRadiusWaveSpawner();

	/** Radius from this actor's location to spawn on the circle (cm). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spinning Radius", meta = (ClampMin = 50.0f, Units = "cm"))
	float SpawnRadius = 400.0f;

	/** Degrees to advance the spawn angle after each spawn (e.g. 45 = 8 positions around the circle). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spinning Radius", meta = (ClampMin = 1.0f, ClampMax = 360.0f, Units = "deg"))
	float AngleStepPerSpawn = 45.0f;

	/** Height offset from the circle plane (Z added to spawn position). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spinning Radius", meta = (Units = "cm"))
	float SpawnHeightOffset = 90.0f;

	virtual FVector GetSpawnLocation_Implementation() override;
	virtual void OnSpawnLocationUsed_Implementation() override;

protected:
	/** Current angle in degrees for the next spawn (0 = +X, 90 = +Y in Unreal). */
	UPROPERTY()
	float CurrentSpawnAngle = 0.0f;
};
