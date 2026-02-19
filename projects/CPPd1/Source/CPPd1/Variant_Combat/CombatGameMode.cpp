// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Combat/CombatGameMode.h"
#include "Variant_Combat/CombatCharacter.h"
#include "Variant_Combat/CombatPlayerController.h"
#include "Variant_Combat/AI/CombatEngagementManager.h"
#include "CPPd1SplitScreenManager.h"
#include "Variant_Combat/AI/CombatWaveSpawner.h"
#include "Variant_Combat/AI/CombatEnemy.h"
#include "CombatDamageable.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

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
	WaveSpawner->OnEnemySpawned.AddDynamic(this, &ACombatGameMode::OnEnemySpawned);
	WaveSpawner->OnWaveCompleted.AddDynamic(this, &ACombatGameMode::OnWaveCompleted);
	WaveSpawner->OnAllWavesCompleted.AddDynamic(this, &ACombatGameMode::OnAllWavesCompleted);
}

void ACombatGameMode::OnEnemySpawned(ACombatEnemy* Enemy)
{
	if (EngagementManager && Enemy)
		EngagementManager->RegisterEnemy(Enemy);
}

void ACombatGameMode::OnWaveCompleted(int32 WaveIndex)
{
	// Restore the gods: heal all player characters (duo ninjas) after each wave cleared
	UWorld* World = GetWorld();
	if (!World || HealPerWaveCleared <= 0.0f) return;
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		if (APawn* Pawn = It->Get()->GetPawn())
		{
			if (ICombatDamageable* Damageable = Cast<ICombatDamageable>(Pawn))
				Damageable->ApplyHealing(HealPerWaveCleared, this);
		}
	}
}

void ACombatGameMode::OnAllWavesCompleted()
{
	// Full restoration: heal all player gods to max (realm is restored)
	UWorld* World = GetWorld();
	if (!World) return;
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		if (APawn* Pawn = It->Get()->GetPawn())
		{
			if (ACombatCharacter* Char = Cast<ACombatCharacter>(Pawn))
			{
				// Heal to full (use a large value; ApplyHealing clamps to MaxHP)
				Char->ApplyHealing(999.0f, this);
			}
		}
	}
	// NPCs congratulate and worship: broadcast so Blueprint/level can trigger NPC behavior
	OnGodsDefendedRealm.Broadcast();
}