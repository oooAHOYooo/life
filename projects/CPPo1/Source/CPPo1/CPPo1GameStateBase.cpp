// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPo1GameStateBase.h"
#include "CPPo1PlayerState.h"

void ACPPo1GameStateBase::SetGamePhase(ECPPo1GamePhase Phase)
{
	GamePhase = Phase;
}

void ACPPo1GameStateBase::StartNextRound()
{
	CurrentRound++;
	GamePhase = ECPPo1GamePhase::Fighting;
}

ACPPo1PlayerState* ACPPo1GameStateBase::GetPlayerStateByIndex(int32 PlayerIndex) const
{
	for (APlayerState* PS : PlayerArray)
	{
		if (ACPPo1PlayerState* CPPo1PS = Cast<ACPPo1PlayerState>(PS))
		{
			if (CPPo1PS->GetPlayerIndex() == PlayerIndex)
			{
				return CPPo1PS;
			}
		}
	}
	return nullptr;
}
