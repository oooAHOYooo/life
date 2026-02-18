// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CPPd1GameStateBase.generated.h"

/** Phase of the round (ninja / dojo style) */
UENUM(BlueprintType)
enum class ECPPd1GamePhase : uint8
{
	/** Waiting to start or between rounds */
	Waiting,
	/** Round in progress */
	Fighting,
	/** Round ended, someone reached the goal */
	RoundEnded
};

/**
 * Game state for CPPd1: game phase, current round, and player state lookup by index.
 */
UCLASS(abstract)
class ACPPd1GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	/** Current phase of the game */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPPd1")
	ECPPd1GamePhase GamePhase = ECPPd1GamePhase::Waiting;

	/** Current round number (1-based) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPPd1", meta = (ClampMin = 0))
	int32 CurrentRound = 0;

	/** Get player state by player index (0 or 1). Returns nullptr if invalid. */
	UFUNCTION(BlueprintCallable, Category = "CPPd1")
	class ACPPd1PlayerState* GetPlayerStateByIndex(int32 PlayerIndex) const;

	/** Set game phase (Blueprint / C++). */
	UFUNCTION(BlueprintCallable, Category = "CPPd1")
	void SetGamePhase(ECPPd1GamePhase Phase);

	/** Advance to next round and set phase to Fighting. */
	UFUNCTION(BlueprintCallable, Category = "CPPd1")
	void StartNextRound();
};
