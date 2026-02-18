// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPd1GameMode.h"
#include "CPPd1Character.h"
#include "CPPd1PlayerController.h"

ACPPd1GameMode::ACPPd1GameMode()
{
	// Set default pawn class to CPPd1Character
	DefaultPawnClass = ACPPd1Character::StaticClass();
	
	// Set default player controller
	PlayerControllerClass = ACPPd1PlayerController::StaticClass();
}
