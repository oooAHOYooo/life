// Copyright Epic Games, Inc. All Rights Reserved.
// CoPlago Game State – shared state for both players (scores, round, phase).
// Replicated for multiplayer; for local 2P you can use for UI and win/lose logic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CoPlagoGameStateBase.generated.h"

class ACoPlagoPlayerState;

UENUM(BlueprintType)
enum class ECoPlagoGamePhase : uint8
{
	WaitingToStart,
	Playing,
	RoundEnd,
	GameOver
};

UCLASS()
class CoPlago_API ACoPlagoGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACoPlagoGameStateBase();

	UPROPERTY(BlueprintReadOnly, Category = "CoPlago")
	ECoPlagoGamePhase GamePhase = ECoPlagoGamePhase::WaitingToStart;

	UPROPERTY(BlueprintReadOnly, Category = "CoPlago")
	int32 CurrentRound = 1;

	/** Get PlayerState for player index (0 or 1). Cast to ACoPlagoPlayerState for score/index. */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	APlayerState* GetPlayerStateByIndex(int32 PlayerIndex) const;

	/** Set phase (e.g. from GameMode when round ends). */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void SetGamePhase(ECoPlagoGamePhase Phase);

	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void SetCurrentRound(int32 Round);
};
