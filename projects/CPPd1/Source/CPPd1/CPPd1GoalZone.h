// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPd1GoalZone.generated.h"

class UBoxComponent;

/** First player to touch this zone wins the round (e.g. reach the scroll / exit). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCPPd1RoundWon, int32, WinningPlayerIndex);

/**
 * Trigger volume: first player (with CPPd1PlayerState) to overlap wins the round.
 * Notifies game state and can be used to advance rounds or end the match.
 */
UCLASS(abstract)
class ACPPd1GoalZone : public AActor
{
	GENERATED_BODY()

public:

	ACPPd1GoalZone();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> TriggerBox;

	/** Fired when a player touches the goal (passes their player index). */
	UPROPERTY(BlueprintAssignable, Category = "CPPd1")
	FOnCPPd1RoundWon OnRoundWon;

	/** If true, only the first overlap wins; further touches are ignored until round is reset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPd1")
	bool bOneWinPerRound = true;

	/** Whether this round already had a winner (reset when starting next round). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPPd1")
	bool bRoundAlreadyWon = false;

	/** Reset state so the zone can trigger again next round. Call from game mode when starting a new round. */
	UFUNCTION(BlueprintCallable, Category = "CPPd1")
	void ResetForNewRound();

protected:

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
