// Copyright Epic Games, Inc. All Rights Reserved.

#include "NinjaGameMode.h"
#include "NinjaCharacter.h"
#include "CubeNinjaCharacter.h"
#include "NinjaPlayerController.h"

ANinjaGameMode::ANinjaGameMode()
{
	// Use CubeNinjaCharacter as default - procedural cube body ninja
	// If you get crashes, temporarily switch to ANinjaCharacter::StaticClass() for testing
	DefaultPawnClass = ACubeNinjaCharacter::StaticClass();
	PlayerControllerClass = ANinjaPlayerController::StaticClass();

	// Enable split screen by default
	bEnableSplitScreen = true;
	bUseVerticalSplit = true; // Vertical split (left/right)
	bForceSplitScreenForSolo = true; // Even solo play uses split screen (for ghost partner)
	NumSplitScreenPlayers = 2;
}
