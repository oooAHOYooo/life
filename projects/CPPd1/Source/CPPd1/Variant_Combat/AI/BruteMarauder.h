// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Variant_Combat/AI/CombatEnemy.h"
#include "BruteMarauder.generated.h"

/**
 * Marauder type: brutes. Big, tough, no weapons—they want to wrestle and ask you to put the weapons down.
 * You can still use the sword, but fighting them unarmed is more enjoyable. Tuned: high HP, slower, heavy hits.
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API ABruteMarauder : public ACombatEnemy
{
	GENERATED_BODY()

public:
	ABruteMarauder();

	/** When true, brute is in "wrestle" mode (for UI/audio: "put weapons down"). Still damageable by sword. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brute")
	bool bPrefersUnarmed = true;
};
