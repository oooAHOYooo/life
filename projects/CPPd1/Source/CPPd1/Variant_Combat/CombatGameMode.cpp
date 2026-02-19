// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Combat/CombatGameMode.h"
#include "Variant_Combat/CombatCharacter.h"
#include "Variant_Combat/CombatPlayerController.h"
#include "Variant_Combat/AI/CombatEngagementManager.h"
#include "CPPd1SplitScreenManager.h"
#include "Variant_Combat/AI/CombatWaveSpawner.h"
#include "Variant_Combat/AI/CombatEnemy.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

ACombatGameMode::ACombatGameMode()
{
	// Set default pawn class to CombatCharacter
	DefaultPawnClass = ACombatCharacter::StaticClass();
	
	// Set default player controller
	PlayerControllerClass = ACombatPlayerController::StaticClass();

	// Create engagement manager component
	EngagementManager = CreateDefaultSubobject<UCombatEngagementManager>(TEXT("EngagementManager"));

	// Create split-screen manager component
	SplitScreenManager = CreateDefaultSubobject<UCPPd1SplitScreenManager>(TEXT("SplitScreenManager"));
}

void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitializeEngagementManager();
	InitializeSplitScreen();

	// Connect wave spawner if set
	if (WaveSpawner)
	{
		RegisterWaveSpawner(WaveSpawner);
	}
}

void ACombatGameMode::InitializeEngagementManager()
{
	if (!EngagementManager)
	{
		return;
	}

	// Engagement manager will auto-find player characters in BeginPlay
}

void ACombatGameMode::InitializeSplitScreen()
{
	if (bEnableSplitScreen)
	{
		SetupSplitScreen();
	}
}

void ACombatGameMode::SetupSplitScreen()
{
	UWorld* World = GetWorld();
	if (!World || !World->GetGameInstance())
	{
		return;
	}

	// Split-screen is handled by the SplitScreenManager component
	if (SplitScreenManager)
	{
		SplitScreenManager->SetupSplitScreenForPlayers();
	}
}

void ACombatGameMode::RegisterWaveSpawner(ACombatWaveSpawner* Spawner)
{
	if (!Spawner || !EngagementManager)
	{
		return;
	}

	WaveSpawner = Spawner;

	// Connect wave spawner events to engagement manager
	WaveSpawner->OnWaveStarted.AddDynamic(this, &ACombatGameMode::OnWaveStarted);
}

void ACombatGameMode::OnWaveStarted(int32 WaveIndex)
{
	// When a wave starts, register all enemies to engagement manager
	if (WaveSpawner && EngagementManager)
	{
		TArray<ACombatEnemy*> WaveEnemies = WaveSpawner->GetCurrentWaveEnemies();
		EngagementManager->RegisterEnemies(WaveEnemies);
	}
}