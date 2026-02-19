// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatTrickSystem.h"
#include "Animation/AnimMontage.h"

UCombatTrickSystem::UCombatTrickSystem()
{
	MaxInputGap = 0.5f;
}

void UCombatTrickSystem::RegisterTrick(const FCombatTrick& Trick)
{
	RegisteredTricks.Add(Trick);
}

bool UCombatTrickSystem::CheckTrickInput(const FCombatTrickInput& Input, float DeltaTime)
{
	TimeSinceLastInput += DeltaTime;

	// If too much time has passed, reset sequence
	if (TimeSinceLastInput > MaxInputGap)
	{
		ClearInputSequence();
	}

	// Add new input
	CurrentInputSequence.Add(Input);
	TimeSinceLastInput = 0.0f;

	// Limit sequence length
	if (CurrentInputSequence.Num() > 10)
	{
		CurrentInputSequence.RemoveAt(0);
	}

	return true;
}

FCombatTrick UCombatTrickSystem::GetBestMatchingTrick(int32 CurrentSkillLevel) const
{
	FCombatTrick BestTrick;
	float BestMatchScore = 0.0f;

	for (const FCombatTrick& Trick : RegisteredTricks)
	{
		// Check skill requirement
		if (Trick.RequiredSkillLevel > CurrentSkillLevel)
		{
			continue;
		}

		// Check if input sequence matches
		if (Trick.InputSequence.Num() > CurrentInputSequence.Num())
		{
			continue;
		}

		// Calculate match score
		float MatchScore = 0.0f;
		bool bMatches = true;

		for (int32 i = 0; i < Trick.InputSequence.Num(); ++i)
		{
			int32 SequenceIndex = CurrentInputSequence.Num() - Trick.InputSequence.Num() + i;
			if (SequenceIndex < 0 || SequenceIndex >= CurrentInputSequence.Num())
			{
				bMatches = false;
				break;
			}

			const FCombatTrickInput& TrickInput = Trick.InputSequence[i];
			const FCombatTrickInput& CurrentInput = CurrentInputSequence[SequenceIndex];

			if (TrickInput.Direction == CurrentInput.Direction && TrickInput.Button == CurrentInput.Button)
			{
				MatchScore += 1.0f;
			}
			else
			{
				bMatches = false;
				break;
			}
		}

		if (bMatches && MatchScore > BestMatchScore)
		{
			BestMatchScore = MatchScore;
			BestTrick = Trick;
		}
	}

	return BestTrick;
}

void UCombatTrickSystem::ClearInputSequence()
{
	CurrentInputSequence.Empty();
	TimeSinceLastInput = 0.0f;
	TrickChainCount = 0;
}

float UCombatTrickSystem::GetFlowMultiplier() const
{
	// Flow multiplier increases with trick chain
	return 1.0f + (TrickChainCount * 0.1f);
}
