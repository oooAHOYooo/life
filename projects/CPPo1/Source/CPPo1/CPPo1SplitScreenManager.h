// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPo1.h"
#include "Components/ActorComponent.h"
#include "CPPo1SplitScreenManager.generated.h"

class APlayerController;

/**
 * Manages vertical split-screen for 2 players (always on, even for solo play)
 */
UCLASS(ClassGroup = (CPPo1), meta = (BlueprintSpawnableComponent))
class CPPo1_API UCPPo1SplitScreenManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UCPPo1SplitScreenManager();

	virtual void BeginPlay() override;

	/** Enable split-screen mode (vertical split) */
	UFUNCTION(BlueprintCallable, Category = "Split Screen")
	void EnableSplitScreen();

	/** Disable split-screen mode (single viewport) */
	UFUNCTION(BlueprintCallable, Category = "Split Screen")
	void DisableSplitScreen();

	/** Setup split-screen for players */
	UFUNCTION(BlueprintCallable, Category = "Split Screen")
	void SetupSplitScreenForPlayers();

	/** Check if split-screen is enabled */
	UFUNCTION(BlueprintPure, Category = "Split Screen")
	bool IsSplitScreenEnabled() const { return bSplitScreenEnabled; }

protected:
	/** Whether split-screen is currently enabled */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Split Screen")
	bool bSplitScreenEnabled = false;

	/** Force split-screen even with 1 player (for solo play) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split Screen")
	bool bForceSplitScreenForSolo = true;

	/** Split-screen layout type (Vertical = Left/Right, Horizontal = Top/Bottom) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split Screen")
	bool bUseVerticalSplit = true; // true = vertical (left/right), false = horizontal (top/bottom)
};
