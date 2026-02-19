// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Variant_Combat/AI/CombatEnemy.h"
#include "TricksterMarauder.generated.h"

/**
 * Marauder type: tricksters. Slippery, hard to catch—they dodge and feint; landing a hit feels earned.
 * Tuned: faster movement, lower HP. Use in wave configs / spinning radius spawner.
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API ATricksterMarauder : public ACombatEnemy
{
	GENERATED_BODY()

public:
	ATricksterMarauder();
};
