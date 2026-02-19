// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "CPPd1GameModeBase.h"
#include "CombatGameMode.generated.h"

class UCombatEngagementManager;
class UCPPd1SplitScreenManager;
class ACombatWaveSpawner;
class ACombatEnemy;

/**
 *  Combat GameMode with wave-based enemies, sequential engagement, and split-screen support
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API ACombatGameMode : public ACPPd1GameModeBase
{
	GENERATED_BODY()
	
public:

	ACombatGameMode();

	virtual void BeginPlay() override;

	/** Enable split-screen (vertical by default, always on even for solo) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPd1|Split Screen")
	bool bEnableSplitScreen = true;

	/** Use vertical split (left/right) instead of horizontal (top/bottom) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPd1|Split Screen")
	bool bUseVerticalSplit = true;

	/** Force split-screen even with 1 player (solo play) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPd1|Split Screen")
	bool bForceSplitScreenForSolo = true;

	/** Number of players for split-screen (2 by default) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPd1|Split Screen", meta = (ClampMin = 1, ClampMax = 2))
	int32 NumSplitScreenPlayers = 2;

	/** Engagement manager component (manages sequential enemy fights) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPPd1|Combat")
	TObjectPtr<UCombatEngagementManager> EngagementManager;

	/** Split-screen manager component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPPd1|Split Screen")
	TObjectPtr<UCPPd1SplitScreenManager> SplitScreenManager;

	/** Wave spawner reference (set in Blueprint or level) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPd1|Waves")
	TObjectPtr<ACombatWaveSpawner> WaveSpawner;

	/** HUD widget class to spawn for players */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPd1|UI")
	TSubclassOf<class UCombatHUD> HUDWidgetClass;

	/** Setup split-screen viewports */
	UFUNCTION(BlueprintCallable, Category = "CPPd1|Split Screen")
	void SetupSplitScreen();

	/** Register wave spawner and connect to engagement manager */
	UFUNCTION(BlueprintCallable, Category = "CPPd1|Waves")
	void RegisterWaveSpawner(ACombatWaveSpawner* Spawner);

	/** Called when an enemy is spawned - register with engagement manager (one-on-one fights) */
	UFUNCTION()
	void OnEnemySpawned(ACombatEnemy* Enemy);

	/** Healing the gods receive when they clear a wave (restoration of damage done by marauders). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPd1|Restoration", meta = (ClampMin = 0.0f, Units = "HP"))
	float HealPerWaveCleared = 1.0f;

	/** When all waves are cleared, broadcast this so NPCs can congratulate and worship the gods. */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGodsDefendedRealm);
	UPROPERTY(BlueprintAssignable, Category = "CPPd1|Restoration")
	FOnGodsDefendedRealm OnGodsDefendedRealm;

	/** Called when a wave is cleared - heal all player gods (restoration). */
	UFUNCTION()
	void OnWaveCompleted(int32 WaveIndex);

	/** Called when all waves are cleared - full restoration, then fire OnGodsDefendedRealm for NPCs. */
	UFUNCTION()
	void OnAllWavesCompleted();

protected:

	/** Initialize engagement manager */
	void InitializeEngagementManager();

	/** Initialize split-screen */
	void InitializeSplitScreen();
};
