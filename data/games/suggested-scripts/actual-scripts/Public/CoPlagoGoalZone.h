// Copyright Epic Games, Inc. All Rights Reserved.
// CoPlago Goal Zone – place in level; when a CoPlago character overlaps, that player wins the round.
// Game Mode’s OnRoundEnd(WinnerPlayerIndex) is called; use RestartRound() to respawn and play again.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoPlagoGoalZone.generated.h"

class UBoxComponent;
class ACoPlagoGameModeBase;
class ACoPlagoCharacter;

UCLASS()
class CoPlago_API ACoPlagoGoalZone : public AActor
{
	GENERATED_BODY()

public:
	ACoPlagoGoalZone();

	/** Collision box: overlap with Pawn. Resize in editor. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoPlago")
	UBoxComponent* TriggerBox;

	/** If true, first player to touch wins the round and round ends. If false, you can use for collectibles (no round end). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoPlago")
	bool bEndRoundOnTouch = true;

	/** Points to add to the overlapping player (optional). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoPlago")
	int32 ScoreToAdd = 1;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
