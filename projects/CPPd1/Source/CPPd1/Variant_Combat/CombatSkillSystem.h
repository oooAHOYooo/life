// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatSkillSystem.generated.h"

/** Combat skill types (inspired by UFC 5) */
UENUM(BlueprintType)
enum class ECombatSkillType : uint8
{
	Striking,		// Punching, kicking accuracy and power
	Grappling,		// Takedowns, submissions
	Defense,		// Blocking, parrying, dodging
	Stamina,		// Endurance and recovery
	Technique,		// Combo execution, timing
	Flow			// Style points, trick chaining (Skate-inspired)
};

/** Skill level data */
USTRUCT(BlueprintType)
struct FCombatSkillData
{
	GENERATED_BODY()

	/** Current skill level (0-100) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SkillLevel = 0.0f;

	/** Experience points toward next level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Experience = 0.0f;

	/** Experience needed for next level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExperienceToNextLevel = 100.0f;

	FCombatSkillData()
		: SkillLevel(0.0f)
		, Experience(0.0f)
		, ExperienceToNextLevel(100.0f)
	{}
};

/**
 * Skill-based combat system inspired by UFC 5
 * Tracks player skill progression and affects combat effectiveness
 */
UCLASS(ClassGroup=(CPPd1), meta=(BlueprintSpawnableComponent))
class UCombatSkillSystem : public UActorComponent
{
	GENERATED_BODY()

public:

	UCombatSkillSystem();

	/** Get skill level for a specific skill type */
	UFUNCTION(BlueprintPure, Category = "Combat|Skill")
	float GetSkillLevel(ECombatSkillType SkillType) const;

	/** Add experience to a skill */
	UFUNCTION(BlueprintCallable, Category = "Combat|Skill")
	void AddExperience(ECombatSkillType SkillType, float Amount);

	/** Get damage multiplier based on striking skill */
	UFUNCTION(BlueprintPure, Category = "Combat|Skill")
	float GetStrikingDamageMultiplier() const;

	/** Get defense effectiveness based on defense skill */
	UFUNCTION(BlueprintPure, Category = "Combat|Skill")
	float GetDefenseEffectiveness() const;

	/** Get stamina efficiency based on stamina skill */
	UFUNCTION(BlueprintPure, Category = "Combat|Skill")
	float GetStaminaEfficiency() const;

	/** Get technique accuracy (affects combo execution) */
	UFUNCTION(BlueprintPure, Category = "Combat|Skill")
	float GetTechniqueAccuracy() const;

	/** Get flow multiplier (affects trick chaining) */
	UFUNCTION(BlueprintPure, Category = "Combat|Skill")
	float GetFlowMultiplier() const;

	/** Get all skill data */
	UFUNCTION(BlueprintPure, Category = "Combat|Skill")
	TMap<ECombatSkillType, FCombatSkillData> GetAllSkills() const { return Skills; }

protected:

	/** Skill data map */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Skill")
	TMap<ECombatSkillType, FCombatSkillData> Skills;

	/** Base damage multiplier at skill level 0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Skill")
	float BaseDamageMultiplier = 0.5f;

	/** Max damage multiplier at skill level 100 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Skill")
	float MaxDamageMultiplier = 2.0f;

	/** Base defense effectiveness at skill level 0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Skill")
	float BaseDefenseEffectiveness = 0.3f;

	/** Max defense effectiveness at skill level 100 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Skill")
	float MaxDefenseEffectiveness = 0.9f;

	virtual void BeginPlay() override;

	/** Check and level up skills */
	void CheckSkillLevelUp(ECombatSkillType SkillType);
};
