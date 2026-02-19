// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Variant_Combat/CombatPlayerController.h"
#include "NinjaPlayerController.generated.h"

/**
 * Player controller for the ninja game. Uses NinjaCharacter for spawn and respawn (P1, P2, ghost).
 */
UCLASS()
class CPPd1_API ANinjaPlayerController : public ACombatPlayerController
{
	GENERATED_BODY()

public:
	ANinjaPlayerController();
};
