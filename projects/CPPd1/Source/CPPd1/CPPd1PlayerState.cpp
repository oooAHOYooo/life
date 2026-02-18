// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPd1PlayerState.h"
#include "Net/UnrealNetwork.h"

ACPPd1PlayerState::ACPPd1PlayerState()
{
	SetReplicates(true);
	bReplicateUsingRegisteredSubObjectList = false;
}

void ACPPd1PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPPd1PlayerState, PlayerIndex);
	DOREPLIFETIME(ACPPd1PlayerState, Honor);
}

void ACPPd1PlayerState::SetPlayerIndex(int32 Index)
{
	PlayerIndex = FMath::Clamp(Index, 0, 1);
}

void ACPPd1PlayerState::AddHonor(int32 Delta)
{
	Honor = FMath::Max(0, Honor + Delta);
}

void ACPPd1PlayerState::SetHonor(int32 NewHonor)
{
	Honor = FMath::Max(0, NewHonor);
}
