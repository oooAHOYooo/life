// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPPd1GameModeBase.generated.h"

/**
 * Game mode base for CPPd1 (ninja-style): 2 players, spawn at starts, round/restart, RequestRespawn on death.
 */
UCLASS(abstract)
class ACPPd1GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ACPPd1GameModeBase();

	/** Delay before destroying the dead pawn (controller's OnPawnDestroyed will then respawn). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPd1|Respawn", meta = (ClampMin = 0, Units = "s"))
	float RespawnDelay = 3.0f;

	/** Request a respawn for the given controller after RespawnDelay. The dying pawn is destroyed so the controller can respawn it. */
	UFUNCTION(BlueprintCallable, Category = "CPPd1")
	void RequestRespawn(AController* Controller, APawn* DyingPawn);

	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Spawns a ghost character for the single player when playing solo (2P co-op by default). */
	virtual void RestartPlayer(AController* NewPlayer) override;

	/** Call after respawning a pawn (e.g. from controller) to spawn ghost when playing solo. */
	UFUNCTION(BlueprintCallable, Category = "CPPd1")
	void EnsureGhostForSoloPlayer(AController* Controller);

protected:

	FTimerHandle RespawnTimer;

	UFUNCTION(BlueprintCallable, Category = "CPPd1")
	void OnRespawnTimerFired(APawn* DyingPawn);
};
