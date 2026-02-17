// Copyright Epic Games, Inc. All Rights Reserved.
// P2G4W Player Controller – minimal; use for per-player UI or camera (split-screen). GDD 6e.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P2G4WPlayerController.generated.h"

UCLASS()
class P2G4W_API AP2G4WPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Optional: force split-screen for this player (enable in Project Settings > Local Multiplayer). */
	UPROPERTY(EditDefaultsOnly, Category = "P2G4W")
	bool bUseSplitScreen = true;

	/** Get our P2G4W player index (0 or 1). */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	int32 GetP2G4WPlayerIndex() const;
};
