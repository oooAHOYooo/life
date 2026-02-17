// Copyright Epic Games, Inc. All Rights Reserved.
// CoPlago Player Controller – minimal; use for per-player UI or camera (split-screen). GDD 6e.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CoPlagoPlayerController.generated.h"

UCLASS()
class CoPlago_API ACoPlagoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Optional: force split-screen for this player (enable in Project Settings > Local Multiplayer). */
	UPROPERTY(EditDefaultsOnly, Category = "CoPlago")
	bool bUseSplitScreen = true;

	/** Get our CoPlago player index (0 or 1). */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	int32 GetCoPlagoPlayerIndex() const;
};
