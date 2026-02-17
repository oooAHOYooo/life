// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoPlagoPlayerState.h"
#include "Net/UnrealNetwork.h"

void ACoPlagoPlayerState::AddScore(int32 Delta)
{
	Score = FMath::Max(0, Score + Delta);
}

void ACoPlagoPlayerState::SetPlayerIndex(int32 Index)
{
	PlayerIndex = Index;
}

void ACoPlagoPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACoPlagoPlayerState, PlayerIndex);
	DOREPLIFETIME(ACoPlagoPlayerState, Score);
}

void ACoPlagoPlayerState::OnRep_Score()
{
}
