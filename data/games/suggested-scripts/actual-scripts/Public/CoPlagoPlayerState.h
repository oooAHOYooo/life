// Copyright Epic Games, Inc. All Rights Reserved.
// CoPlago Player State – per-player data (score, player index). Use for coop or versus (GDD 6e).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CoPlagoPlayerState.generated.h"

UCLASS()
class CoPlago_API ACoPlagoPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/** 0 or 1 for local 2P. Set by GameMode when player joins. */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CoPlago")
	int32 PlayerIndex = 0;

	/** Optional score (coop shared goal or versus). */
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Score, Category = "CoPlago")
	int32 Score = 0;

	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void AddScore(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void SetPlayerIndex(int32 Index);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_Score();
};
