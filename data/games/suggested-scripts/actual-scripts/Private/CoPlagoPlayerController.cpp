// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoPlagoPlayerController.h"
#include "CoPlagoPlayerState.h"

int32 ACoPlagoPlayerController::GetCoPlagoPlayerIndex() const
{
	ACoPlagoPlayerState* PS = GetPlayerState<ACoPlagoPlayerState>();
	return PS ? PS->PlayerIndex : 0;
}
