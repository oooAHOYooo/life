// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPo1SplitScreenManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

UCPPo1SplitScreenManager::UCPPo1SplitScreenManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCPPo1SplitScreenManager::BeginPlay()
{
	Super::BeginPlay();

	// Enable split-screen on begin play
	if (bForceSplitScreenForSolo)
	{
		EnableSplitScreen();
	}
}

void UCPPo1SplitScreenManager::EnableSplitScreen()
{
	if (bSplitScreenEnabled)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World || !World->GetGameInstance())
	{
		return;
	}

	// Split screen is handled by GameMode's NumPlayers setting
	// This component just tracks the state
	// The actual split screen setup happens in GameMode
	
	bSplitScreenEnabled = true;
}

void UCPPo1SplitScreenManager::DisableSplitScreen()
{
	bSplitScreenEnabled = false;
}

void UCPPo1SplitScreenManager::SetupSplitScreenForPlayers()
{
	EnableSplitScreen();
}
