// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPo1GameModeBase.h"
#include "CPPo1GameMode.generated.h"

/**
 *  Simple GameMode for a third person game (inherits 2P, rounds, RequestRespawn from CPPo1GameModeBase).
 */
UCLASS(BlueprintType, Blueprintable)
class ACPPo1GameMode : public ACPPo1GameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ACPPo1GameMode();
};



