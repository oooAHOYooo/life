// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPo1PlayerState.h"
#include "Net/UnrealNetwork.h"

ACPPo1PlayerState::ACPPo1PlayerState()
{
	SetReplicates(true);
	bReplicateUsingRegisteredSubObjectList = false;
}

void ACPPo1PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPPo1PlayerState, PlayerIndex);
	DOREPLIFETIME(ACPPo1PlayerState, Honor);
}

void ACPPo1PlayerState::SetPlayerIndex(int32 Index)
{
	PlayerIndex = FMath::Clamp(Index, 0, 1);
}

void ACPPo1PlayerState::AddHonor(int32 Delta)
{
	Honor = FMath::Max(0, Honor + Delta);
}

void ACPPo1PlayerState::SetHonor(int32 NewHonor)
{
	Honor = FMath::Max(0, NewHonor);
}
