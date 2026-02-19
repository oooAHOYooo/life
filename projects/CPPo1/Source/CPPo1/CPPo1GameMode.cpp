// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPo1GameMode.h"
#include "CPPo1Character.h"
#include "CPPo1PlayerController.h"

ACPPo1GameMode::ACPPo1GameMode()
{
	// Set default pawn class to CPPo1Character
	DefaultPawnClass = ACPPo1Character::StaticClass();
	
	// Set default player controller
	PlayerControllerClass = ACPPo1PlayerController::StaticClass();
}
