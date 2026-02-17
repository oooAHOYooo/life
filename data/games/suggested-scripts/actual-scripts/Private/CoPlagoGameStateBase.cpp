// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoPlagoGameStateBase.h"
#include "CoPlagoPlayerState.h"
#include "Net/UnrealNetwork.h"

ACoPlagoGameStateBase::ACoPlagoGameStateBase()
{
}

APlayerState* ACoPlagoGameStateBase::GetPlayerStateByIndex(int32 PlayerIndex) const
{
	const TArray<APlayerState*>& PS = PlayerArray;
	if (PlayerIndex >= 0 && PlayerIndex < PS.Num()) return PS[PlayerIndex];
	return nullptr;
}

void ACoPlagoGameStateBase::SetGamePhase(ECoPlagoGamePhase Phase)
{
	GamePhase = Phase;
}

void ACoPlagoGameStateBase::SetCurrentRound(int32 Round)
{
	CurrentRound = Round;
}
