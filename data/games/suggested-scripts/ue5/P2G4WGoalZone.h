// Copyright Epic Games, Inc. All Rights Reserved.
// P2G4W Goal Zone – place in level; when a P2G4W character overlaps, that player wins the round.
// Game Mode’s OnRoundEnd(WinnerPlayerIndex) is called; use RestartRound() to respawn and play again.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P2G4WGoalZone.generated.h"

class UBoxComponent;
class AP2G4WGameModeBase;
class AP2G4WCharacter;

UCLASS()
class P2G4W_API AP2G4WGoalZone : public AActor
{
	GENERATED_BODY()

public:
	AP2G4WGoalZone();

	/** Collision box: overlap with Pawn. Resize in editor. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "P2G4W")
	UBoxComponent* TriggerBox;

	/** If true, first player to touch wins the round and round ends. If false, you can use for collectibles (no round end). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P2G4W")
	bool bEndRoundOnTouch = true;

	/** Points to add to the overlapping player (optional). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P2G4W")
	int32 ScoreToAdd = 1;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
