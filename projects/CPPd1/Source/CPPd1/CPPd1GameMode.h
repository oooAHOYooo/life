// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1GameModeBase.h"
#include "CPPd1GameMode.generated.h"

/**
 *  Simple GameMode for a third person game (inherits 2P, rounds, RequestRespawn from CPPd1GameModeBase).
 */
UCLASS(BlueprintType, Blueprintable)
class ACPPd1GameMode : public ACPPd1GameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ACPPd1GameMode();
};



