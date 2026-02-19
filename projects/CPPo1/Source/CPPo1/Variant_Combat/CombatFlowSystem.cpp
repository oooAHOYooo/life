// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatFlowSystem.h"

UCombatFlowSystem::UCombatFlowSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentFlow = 0.0f;
}

void UCombatFlowSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Decay flow over time
	if (CurrentFlow > 0.0f)
	{
		CurrentFlow = FMath::Max(0.0f, CurrentFlow - (FlowDecayRate * DeltaTime));
	}

	// Reset combo if too much time has passed
	TimeSinceLastCombo += DeltaTime;
	if (TimeSinceLastCombo > ComboResetTime && ComboCount > 0)
	{
		ResetCombo();
	}
}

void UCombatFlowSystem::AddFlowPoints(float Amount)
{
	CurrentFlow = FMath::Min(MaxFlow, CurrentFlow + Amount);
}

float UCombatFlowSystem::GetFlowMultiplier() const
{
	if (IsInFlowState())
	{
		return FlowStateMultiplier;
	}
	return 1.0f;
}

void UCombatFlowSystem::IncrementCombo()
{
	ComboCount++;
	TimeSinceLastCombo = 0.0f;
	
	// Add flow points for combos
	AddFlowPoints(0.05f * ComboCount); // More flow for longer combos
}

void UCombatFlowSystem::ResetCombo()
{
	ComboCount = 0;
	TimeSinceLastCombo = 0.0f;
}

void UCombatFlowSystem::BeginPlay()
{
	Super::BeginPlay();
}
