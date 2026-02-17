// Copyright Epic Games, Inc. All Rights Reserved.
// P2G4W Game State – shared state for both players (scores, round, phase).
// Replicated for multiplayer; for local 2P you can use for UI and win/lose logic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "P2G4WGameStateBase.generated.h"

class AP2G4WPlayerState;

UENUM(BlueprintType)
enum class EP2G4WGamePhase : uint8
{
	WaitingToStart,
	Playing,
	RoundEnd,
	GameOver
};

UCLASS()
class P2G4W_API AP2G4WGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AP2G4WGameStateBase();

	UPROPERTY(BlueprintReadOnly, Category = "P2G4W")
	EP2G4WGamePhase GamePhase = EP2G4WGamePhase::WaitingToStart;

	UPROPERTY(BlueprintReadOnly, Category = "P2G4W")
	int32 CurrentRound = 1;

	/** Get PlayerState for player index (0 or 1). Cast to AP2G4WPlayerState for score/index. */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	APlayerState* GetPlayerStateByIndex(int32 PlayerIndex) const;

	/** Set phase (e.g. from GameMode when round ends). */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	void SetGamePhase(EP2G4WGamePhase Phase);

	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	void SetCurrentRound(int32 Round);
};
