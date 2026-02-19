// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Variant_Combat/CombatGameMode.h"
#include "NinjaGameMode.generated.h"

/**
 * Combat game mode with cube ninja as default character. P1 and P2 (and ghost when solo) use CubeNinjaCharacter.
 * CubeNinjaCharacter uses procedural cube body instead of skeletal mesh.
 * Place CombatWaveSpawner in the level and set Enemy Class to NinjaEnemy (or a Blueprint) for waves.
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API ANinjaGameMode : public ACombatGameMode
{
	GENERATED_BODY()

public:
	ANinjaGameMode();
};
