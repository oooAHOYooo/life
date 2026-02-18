// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPd1GameStateBase.h"
#include "CPPd1PlayerState.h"

void ACPPd1GameStateBase::SetGamePhase(ECPPd1GamePhase Phase)
{
	GamePhase = Phase;
}

void ACPPd1GameStateBase::StartNextRound()
{
	CurrentRound++;
	GamePhase = ECPPd1GamePhase::Fighting;
}

ACPPd1PlayerState* ACPPd1GameStateBase::GetPlayerStateByIndex(int32 PlayerIndex) const
{
	for (APlayerState* PS : PlayerArray)
	{
		if (ACPPd1PlayerState* CPPd1PS = Cast<ACPPd1PlayerState>(PS))
		{
			if (CPPd1PS->GetPlayerIndex() == PlayerIndex)
			{
				return CPPd1PS;
			}
		}
	}
	return nullptr;
}
