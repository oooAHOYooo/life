// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatTrickSystem.generated.h"

class UAnimMontage;

/** Directional input for tricks (like Skate) */
UENUM(BlueprintType)
enum class ECombatTrickDirection : uint8
{
	Up,
	Down,
	Left,
	Right,
	UpLeft,
	UpRight,
	DownLeft,
	DownRight,
	Neutral
};

/** Trick input sequence (like Skate's trick system) */
USTRUCT(BlueprintType)
struct FCombatTrickInput
{
	GENERATED_BODY()

	/** Directional input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatTrickDirection Direction = ECombatTrickDirection::Neutral;

	/** Button input (0 = no button, 1 = light attack, 2 = heavy attack, 3 = special) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Button = 0;

	/** Time window for this input (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeWindow = 0.3f;

	FCombatTrickInput()
		: Direction(ECombatTrickDirection::Neutral)
		, Button(0)
		, TimeWindow(0.3f)
	{}
};

/** A combat trick/combo (like Skate's trick system) */
USTRUCT(BlueprintType)
struct FCombatTrick
{
	GENERATED_BODY()

	/** Name of the trick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrickName;

	/** Input sequence to perform this trick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCombatTrickInput> InputSequence;

	/** Animation montage to play */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* TrickMontage;

	/** Damage multiplier for this trick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 1.0f;

	/** Flow/style points awarded */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FlowPoints = 10.0f;

	/** Required skill level to perform */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredSkillLevel = 0;

	FCombatTrick()
		: TrickName(NAME_None)
		, DamageMultiplier(1.0f)
		, FlowPoints(10.0f)
		, RequiredSkillLevel(0)
	{}
};

/**
 * Trick system for combat - inspired by Skate 4's trick system
 * Allows chaining directional inputs + buttons to create combos
 */
UCLASS(BlueprintType, Blueprintable)
class UCombatTrickSystem : public UObject
{
	GENERATED_BODY()

public:

	UCombatTrickSystem();

	/** Register a trick */
	UFUNCTION(BlueprintCallable, Category = "Combat|Trick")
	void RegisterTrick(const FCombatTrick& Trick);

	/** Check if current input sequence matches a trick */
	UFUNCTION(BlueprintCallable, Category = "Combat|Trick")
	bool CheckTrickInput(const FCombatTrickInput& Input, float DeltaTime);

	/** Get the best matching trick from current input sequence */
	UFUNCTION(BlueprintCallable, Category = "Combat|Trick")
	FCombatTrick GetBestMatchingTrick(int32 CurrentSkillLevel) const;

	/** Clear the current input sequence */
	UFUNCTION(BlueprintCallable, Category = "Combat|Trick")
	void ClearInputSequence();

	/** Get current input sequence (for debugging) */
	UFUNCTION(BlueprintPure, Category = "Combat|Trick")
	TArray<FCombatTrickInput> GetCurrentInputSequence() const { return CurrentInputSequence; }

	/** Get flow multiplier based on trick chain */
	UFUNCTION(BlueprintPure, Category = "Combat|Trick")
	float GetFlowMultiplier() const;

protected:

	/** Registered tricks */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Trick")
	TArray<FCombatTrick> RegisteredTricks;

	/** Current input sequence being built */
	TArray<FCombatTrickInput> CurrentInputSequence;

	/** Time since last input */
	float TimeSinceLastInput = 0.0f;

	/** Maximum time between inputs to maintain combo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Trick")
	float MaxInputGap = 0.5f;

	/** Current trick chain count (for flow multiplier) */
	int32 TrickChainCount = 0;
};
