// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPPo1PlayerState.generated.h"

/**
 * Player state for CPPo1: player index (0 or 1) and score (honor).
 */
UCLASS()
class ACPPo1PlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	/** Player index (0 or 1) for 2P. Set by game mode or controller. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "CPPo1", meta = (ClampMin = 0, ClampMax = 1))
	int32 PlayerIndex = 0;

	/** Honor (score) for this player. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "CPPo1", meta = (ClampMin = 0))
	int32 Honor = 0;

	ACPPo1PlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Set player index (e.g. from game mode). */
	UFUNCTION(BlueprintCallable, Category = "CPPo1")
	void SetPlayerIndex(int32 Index);

	/** Get player index. */
	UFUNCTION(BlueprintPure, Category = "CPPo1")
	int32 GetPlayerIndex() const { return PlayerIndex; }

	/** Add to honor. */
	UFUNCTION(BlueprintCallable, Category = "CPPo1")
	void AddHonor(int32 Delta);

	/** Set honor. */
	UFUNCTION(BlueprintCallable, Category = "CPPo1")
	void SetHonor(int32 NewHonor);

	/** Get honor. */
	UFUNCTION(BlueprintPure, Category = "CPPo1")
	int32 GetHonor() const { return Honor; }
};
