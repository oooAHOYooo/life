// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatAdvancedMechanics.h"
#include "CombatStaminaSystem.h"
#include "CombatSkillSystem.h"

UCombatAdvancedMechanics::UCombatAdvancedMechanics()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatAdvancedMechanics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Drain stamina while blocking
	if (bIsBlocking)
	{
		if (UCombatStaminaSystem* StaminaSystem = GetOwner()->FindComponentByClass<UCombatStaminaSystem>())
		{
			StaminaSystem->DrainStamina(BlockStaminaDrain * DeltaTime);
			
			// Stop blocking if out of stamina
			if (StaminaSystem->IsExhausted())
			{
				StopBlocking();
			}
		}
	}

	// Update parry timing
	if (ParryInputTime >= 0.0f)
	{
		ParryInputTime += DeltaTime;
		if (ParryInputTime > GoodParryWindow)
		{
			ParryInputTime = -1.0f;
		}
	}

	// Update counter-attack window
	if (bCanCounterAttack)
	{
		TimeSinceLastParry += DeltaTime;
		if (TimeSinceLastParry > CounterAttackWindow)
		{
			bCanCounterAttack = false;
		}
	}
}

EParryResult UCombatAdvancedMechanics::AttemptParry()
{
	if (ParryInputTime < 0.0f)
	{
		ParryInputTime = 0.0f;
		return EParryResult::Missed;
	}

	if (ParryInputTime <= PerfectParryWindow)
	{
		bCanCounterAttack = true;
		TimeSinceLastParry = 0.0f;
		ParryInputTime = -1.0f;
		return EParryResult::Perfect;
	}
	else if (ParryInputTime <= GoodParryWindow)
	{
		bCanCounterAttack = true;
		TimeSinceLastParry = 0.0f;
		ParryInputTime = -1.0f;
		return EParryResult::Good;
	}

	return EParryResult::Missed;
}

void UCombatAdvancedMechanics::StartBlocking()
{
	bIsBlocking = true;
}

void UCombatAdvancedMechanics::StopBlocking()
{
	bIsBlocking = false;
}

bool UCombatAdvancedMechanics::IsInParryWindow() const
{
	return ParryInputTime >= 0.0f && ParryInputTime <= GoodParryWindow;
}

void UCombatAdvancedMechanics::PerformCounterAttack()
{
	if (bCanCounterAttack)
	{
		// Counter-attack logic would trigger attack with multiplier
		bCanCounterAttack = false;
		TimeSinceLastParry = -1.0f;
	}
}

float UCombatAdvancedMechanics::GetBlockEffectiveness() const
{
	float Effectiveness = BaseBlockEffectiveness;
	
	// Improve with defense skill
	if (UCombatSkillSystem* SkillSystem = GetOwner()->FindComponentByClass<UCombatSkillSystem>())
	{
		Effectiveness = SkillSystem->GetDefenseEffectiveness();
	}

	return Effectiveness;
}

void UCombatAdvancedMechanics::BeginPlay()
{
	Super::BeginPlay();
}
