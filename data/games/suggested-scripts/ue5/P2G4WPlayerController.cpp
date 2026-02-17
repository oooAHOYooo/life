// Copyright Epic Games, Inc. All Rights Reserved.

#include "P2G4WPlayerController.h"
#include "P2G4WPlayerState.h"

int32 AP2G4WPlayerController::GetP2G4WPlayerIndex() const
{
	AP2G4WPlayerState* PS = GetPlayerState<AP2G4WPlayerState>();
	return PS ? PS->PlayerIndex : 0;
}
