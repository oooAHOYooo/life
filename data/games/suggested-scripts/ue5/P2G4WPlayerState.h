// Copyright Epic Games, Inc. All Rights Reserved.
// P2G4W Player State – per-player data (score, player index). Use for coop or versus (GDD 6e).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "P2G4WPlayerState.generated.h"

UCLASS()
class P2G4W_API AP2G4WPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/** 0 or 1 for local 2P. Set by GameMode when player joins. */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "P2G4W")
	int32 PlayerIndex = 0;

	/** Optional score (coop shared goal or versus). */
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Score, Category = "P2G4W")
	int32 Score = 0;

	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	void AddScore(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	void SetPlayerIndex(int32 Index);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_Score();
};
