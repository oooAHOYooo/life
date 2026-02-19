// Copyright Epic Games, Inc. All Rights Reserved.

#include "NinjaPlayerController.h"
#include "NinjaCharacter.h"
#include "CubeNinjaCharacter.h"

ANinjaPlayerController::ANinjaPlayerController()
{
	// Use CubeNinjaCharacter for spawns and respawns
	CharacterClass = ACubeNinjaCharacter::StaticClass();
}
