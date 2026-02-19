// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPo1GoalZone.generated.h"

class UBoxComponent;

/** First player to touch this zone wins the round (e.g. reach the scroll / exit). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCPPo1RoundWon, int32, WinningPlayerIndex);

/**
 * Trigger volume: first player (with CPPo1PlayerState) to overlap wins the round.
 * Notifies game state and can be used to advance rounds or end the match.
 */
UCLASS()
class ACPPo1GoalZone : public AActor
{
	GENERATED_BODY()

public:

	ACPPo1GoalZone();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> TriggerBox;

	/** Fired when a player touches the goal (passes their player index). */
	UPROPERTY(BlueprintAssignable, Category = "CPPo1")
	FOnCPPo1RoundWon OnRoundWon;

	/** If true, only the first overlap wins; further touches are ignored until round is reset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPo1")
	bool bOneWinPerRound = true;

	/** Whether this round already had a winner (reset when starting next round). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPPo1")
	bool bRoundAlreadyWon = false;

	/** Reset state so the zone can trigger again next round. Call from game mode when starting a new round. */
	UFUNCTION(BlueprintCallable, Category = "CPPo1")
	void ResetForNewRound();

protected:

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
