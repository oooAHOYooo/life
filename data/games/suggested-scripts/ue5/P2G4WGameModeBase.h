// Copyright Epic Games, Inc. All Rights Reserved.
// P2G4W Game Mode – creates 2 players, spawns 2 pawns, optional round/session flow.
// Set as default Game Mode in Project Settings or per-level. Place two PlayerStart actors (tag "PlayerStart0", "PlayerStart1") or use offset spawn.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P2G4WGameModeBase.generated.h"

class APlayerStart;
class AP2G4WPlayerState;

UCLASS()
class P2G4W_API AP2G4WGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP2G4WGameModeBase();

	/** Number of players to create (default 2). */
	UPROPERTY(EditDefaultsOnly, Category = "P2G4W|Players")
	int32 NumPlayers = 2;

	/** If true, create second player in BeginPlay. If false, only Player 0 exists until you call EnsureSecondPlayer(). */
	UPROPERTY(EditDefaultsOnly, Category = "P2G4W|Players")
	bool bSpawnSecondPlayerOnBeginPlay = true;

	/** Spawn offset for Player 1 when no PlayerStart1 found (relative to Player 0). */
	UPROPERTY(EditDefaultsOnly, Category = "P2G4W|Spawn")
	FVector SecondPlayerSpawnOffset = FVector(300.f, 0.f, 0.f);

	/** Optional: rounds (0 = no round limit). */
	UPROPERTY(EditDefaultsOnly, Category = "P2G4W|Session")
	int32 MaxRounds = 0;

	/** Current round (1-based). */
	UPROPERTY(BlueprintReadOnly, Category = "P2G4W|Session")
	int32 CurrentRound = 1;

	/** Ensures a second player exists and possesses a pawn. Call from BeginPlay or when ready. */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	void EnsureSecondPlayer();

	virtual void BeginPlay() override;
	virtual void RestartPlayer(AController* NewPlayer) override;

	/** Override to implement win/lose (GDD 6d). Called when you want to end round/game. */
	UFUNCTION(BlueprintNativeEvent, Category = "P2G4W")
	void OnRoundEnd(int32 WinnerPlayerIndex);
	virtual void OnRoundEnd_Implementation(int32 WinnerPlayerIndex);

	/** Override to implement game over / session end. */
	UFUNCTION(BlueprintNativeEvent, Category = "P2G4W")
	void OnGameOver();
	virtual void OnGameOver_Implementation();

	/** Respawn both players at their starts. Call after OnRoundEnd to play again (e.g. from Blueprint timer). */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	void RestartRound();

	/** Request a single-player respawn after Delay seconds (e.g. when character dies). */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	void RequestRespawn(AController* Controller, float Delay);

protected:
	/** Find PlayerStart by tag (e.g. "PlayerStart0", "PlayerStart1"). */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	AActor* FindPlayerStartByTag(int32 PlayerIndex) const;

	UFUNCTION()
	void OnRespawnTimer();

	FTimerHandle RespawnTimerHandle;
	TWeakObjectPtr<AController> PendingRespawnController;
};
