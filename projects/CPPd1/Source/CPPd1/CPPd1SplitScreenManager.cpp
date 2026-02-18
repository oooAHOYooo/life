// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPd1SplitScreenManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

UCPPd1SplitScreenManager::UCPPd1SplitScreenManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCPPd1SplitScreenManager::BeginPlay()
{
	Super::BeginPlay();

	// Enable split-screen on begin play
	if (bForceSplitScreenForSolo)
	{
		EnableSplitScreen();
	}
}

void UCPPd1SplitScreenManager::EnableSplitScreen()
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

void UCPPd1SplitScreenManager::DisableSplitScreen()
{
	bSplitScreenEnabled = false;
}

void UCPPd1SplitScreenManager::SetupSplitScreenForPlayers()
{
	EnableSplitScreen();
}
