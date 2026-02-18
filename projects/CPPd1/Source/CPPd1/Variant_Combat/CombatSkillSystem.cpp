// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatSkillSystem.h"

UCombatSkillSystem::UCombatSkillSystem()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Initialize all skill types
	for (int32 i = 0; i < static_cast<int32>(ECombatSkillType::Flow) + 1; ++i)
	{
		ECombatSkillType SkillType = static_cast<ECombatSkillType>(i);
		Skills.Add(SkillType, FCombatSkillData());
	}
}

float UCombatSkillSystem::GetSkillLevel(ECombatSkillType SkillType) const
{
	if (const FCombatSkillData* SkillData = Skills.Find(SkillType))
	{
		return SkillData->SkillLevel;
	}
	return 0.0f;
}

void UCombatSkillSystem::AddExperience(ECombatSkillType SkillType, float Amount)
{
	if (FCombatSkillData* SkillData = Skills.Find(SkillType))
	{
		SkillData->Experience += Amount;
		CheckSkillLevelUp(SkillType);
	}
}

float UCombatSkillSystem::GetStrikingDamageMultiplier() const
{
	float SkillLevel = GetSkillLevel(ECombatSkillType::Striking);
	float Normalized = SkillLevel / 100.0f;
	return FMath::Lerp(BaseDamageMultiplier, MaxDamageMultiplier, Normalized);
}

float UCombatSkillSystem::GetDefenseEffectiveness() const
{
	float SkillLevel = GetSkillLevel(ECombatSkillType::Defense);
	float Normalized = SkillLevel / 100.0f;
	return FMath::Lerp(BaseDefenseEffectiveness, MaxDefenseEffectiveness, Normalized);
}

float UCombatSkillSystem::GetStaminaEfficiency() const
{
	float SkillLevel = GetSkillLevel(ECombatSkillType::Stamina);
	// Higher stamina skill = less stamina drain
	return 1.0f - (SkillLevel / 200.0f); // 0.5x drain at max level
}

float UCombatSkillSystem::GetTechniqueAccuracy() const
{
	float SkillLevel = GetSkillLevel(ECombatSkillType::Technique);
	// Higher technique = better combo execution window
	return SkillLevel / 100.0f; // 0.0 to 1.0
}

float UCombatSkillSystem::GetFlowMultiplier() const
{
	float SkillLevel = GetSkillLevel(ECombatSkillType::Flow);
	// Higher flow = better trick chaining
	return 1.0f + (SkillLevel / 50.0f); // 1.0x to 3.0x at max
}

void UCombatSkillSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatSkillSystem::CheckSkillLevelUp(ECombatSkillType SkillType)
{
	if (FCombatSkillData* SkillData = Skills.Find(SkillType))
	{
		while (SkillData->Experience >= SkillData->ExperienceToNextLevel && SkillData->SkillLevel < 100.0f)
		{
			SkillData->Experience -= SkillData->ExperienceToNextLevel;
			SkillData->SkillLevel = FMath::Min(100.0f, SkillData->SkillLevel + 1.0f);
			
			// Increase XP needed for next level (exponential)
			SkillData->ExperienceToNextLevel *= 1.1f;
		}
	}
}
