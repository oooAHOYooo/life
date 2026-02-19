// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "Variant_Combat/AI/CombatEnemy.h"
#include "NinjaEnemy.generated.h"

/**
 * Concrete enemy for wave spawner. Lock-on-able, takes damage, uses combo/charged attacks via AI.
 * Assign this (or a Blueprint child) as Wave Config Enemy Class in CombatWaveSpawner.
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API ANinjaEnemy : public ACombatEnemy
{
	GENERATED_BODY()

public:
	ANinjaEnemy();
};
