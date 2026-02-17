// Copyright Epic Games, Inc. All Rights Reserved.

#include "P2G4WGameStateBase.h"
#include "P2G4WPlayerState.h"
#include "Net/UnrealNetwork.h"

AP2G4WGameStateBase::AP2G4WGameStateBase()
{
}

APlayerState* AP2G4WGameStateBase::GetPlayerStateByIndex(int32 PlayerIndex) const
{
	const TArray<APlayerState*>& PS = PlayerArray;
	if (PlayerIndex >= 0 && PlayerIndex < PS.Num()) return PS[PlayerIndex];
	return nullptr;
}

void AP2G4WGameStateBase::SetGamePhase(EP2G4WGamePhase Phase)
{
	GamePhase = Phase;
}

void AP2G4WGameStateBase::SetCurrentRound(int32 Round)
{
	CurrentRound = Round;
}
