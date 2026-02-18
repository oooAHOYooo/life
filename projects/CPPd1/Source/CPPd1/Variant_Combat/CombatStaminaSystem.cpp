// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatStaminaSystem.h"
#include "CombatSkillSystem.h"

UCombatStaminaSystem::UCombatStaminaSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentStamina = MaxStamina;
}

void UCombatStaminaSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastDrain += DeltaTime;

	// Recover stamina if enough time has passed
	if (TimeSinceLastDrain >= RecoveryDelay && CurrentStamina < MaxStamina)
	{
		float RecoveryRate = IsExhausted() ? ExhaustedRecoveryRate : BaseRecoveryRate;
		RecoveryRate *= GetRecoveryRate(); // Apply efficiency multiplier
		
		CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + (RecoveryRate * DeltaTime));
	}
}

void UCombatStaminaSystem::DrainStamina(float Amount)
{
	// Apply stamina efficiency (higher skill = less drain)
	float ActualDrain = Amount * (1.0f / StaminaEfficiency);
	
	CurrentStamina = FMath::Max(0.0f, CurrentStamina - ActualDrain);
	TimeSinceLastDrain = 0.0f;
}

bool UCombatStaminaSystem::HasStamina(float RequiredAmount) const
{
	return CurrentStamina >= RequiredAmount;
}

float UCombatStaminaSystem::GetRecoveryRate() const
{
	return StaminaEfficiency;
}

void UCombatStaminaSystem::BeginPlay()
{
	Super::BeginPlay();

	// Get stamina efficiency from skill system if available
	if (AActor* Owner = GetOwner())
	{
		if (UCombatSkillSystem* SkillSystem = Owner->FindComponentByClass<UCombatSkillSystem>())
		{
			StaminaEfficiency = SkillSystem->GetStaminaEfficiency();
		}
	}
}
