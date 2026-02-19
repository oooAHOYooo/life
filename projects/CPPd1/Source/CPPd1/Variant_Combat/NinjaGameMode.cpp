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
}
