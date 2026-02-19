// Copyright Epic Games, Inc. All Rights Reserved.

#include "NinjaGameMode.h"
#include "NinjaCharacter.h"
#include "CubeNinjaCharacter.h"
#include "NinjaPlayerController.h"

ANinjaGameMode::ANinjaGameMode()
{
	// Use CubeNinjaCharacter as default - procedural cube body ninja
	DefaultPawnClass = ACubeNinjaCharacter::StaticClass();
	PlayerControllerClass = ANinjaPlayerController::StaticClass();
}
