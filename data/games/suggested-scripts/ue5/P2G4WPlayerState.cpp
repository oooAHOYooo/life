// Copyright Epic Games, Inc. All Rights Reserved.

#include "P2G4WPlayerState.h"
#include "Net/UnrealNetwork.h"

void AP2G4WPlayerState::AddScore(int32 Delta)
{
	Score = FMath::Max(0, Score + Delta);
}

void AP2G4WPlayerState::SetPlayerIndex(int32 Index)
{
	PlayerIndex = Index;
}

void AP2G4WPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AP2G4WPlayerState, PlayerIndex);
	DOREPLIFETIME(AP2G4WPlayerState, Score);
}

void AP2G4WPlayerState::OnRep_Score()
{
}
