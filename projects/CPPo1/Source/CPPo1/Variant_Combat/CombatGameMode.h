// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPo1.h"
#include "CPPo1GameModeBase.h"
#include "CombatGameMode.generated.h"

class UCombatEngagementManager;
class UCPPo1SplitScreenManager;
class ACombatWaveSpawner;
class ACombatEnemy;

/**
 *  Combat GameMode with wave-based enemies, sequential engagement, and split-screen support
 */
UCLASS(BlueprintType, Blueprintable)
class CPPo1_API ACombatGameMode : public ACPPo1GameModeBase
{
	GENERATED_BODY()
	
public:

	ACombatGameMode();

	virtual void BeginPlay() override;

	/** Enable split-screen (vertical by default, always on even for solo) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPo1|Split Screen")
	bool bEnableSplitScreen = true;

	/** Use vertical split (left/right) instead of horizontal (top/bottom) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPo1|Split Screen")
	bool bUseVerticalSplit = true;

	/** Force split-screen even with 1 player (solo play) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPo1|Split Screen")
	bool bForceSplitScreenForSolo = true;

	/** Number of players for split-screen (2 by default) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPo1|Split Screen", meta = (ClampMin = 1, ClampMax = 2))
	int32 NumSplitScreenPlayers = 2;

	/** Engagement manager component (manages sequential enemy fights) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPPo1|Combat")
	TObjectPtr<UCombatEngagementManager> EngagementManager;

	/** Split-screen manager component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPPo1|Split Screen")
	TObjectPtr<UCPPo1SplitScreenManager> SplitScreenManager;

	/** Wave spawner reference (set in Blueprint or level) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPo1|Waves")
	TObjectPtr<ACombatWaveSpawner> WaveSpawner;

	/** HUD widget class to spawn for players */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPo1|UI")
	TSubclassOf<class UCombatHUD> HUDWidgetClass;

	/** Setup split-screen viewports */
	UFUNCTION(BlueprintCallable, Category = "CPPo1|Split Screen")
	void SetupSplitScreen();

	/** Register wave spawner and connect to engagement manager */
	UFUNCTION(BlueprintCallable, Category = "CPPo1|Waves")
	void RegisterWaveSpawner(ACombatWaveSpawner* Spawner);

	/** Called when a wave starts - registers enemies to engagement manager */
	UFUNCTION()
	void OnWaveStarted(int32 WaveIndex);

protected:

	/** Initialize engagement manager */
	void InitializeEngagementManager();

	/** Initialize split-screen */
	void InitializeSplitScreen();
};
