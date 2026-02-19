// Copyright Epic Games, Inc. All Rights Reserved.

#include "NinjaGameMode.h"
#include "NinjaCharacter.h"
#include "NinjaPlayerController.h"

ANinjaGameMode::ANinjaGameMode()
{
	DefaultPawnClass = ANinjaCharacter::StaticClass();
	PlayerControllerClass = ANinjaPlayerController::StaticClass();
}
