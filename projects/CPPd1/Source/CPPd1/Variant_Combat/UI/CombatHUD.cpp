// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Combat/UI/CombatHUD.h"
#include "Variant_Combat/CombatCharacter.h"
#include "Variant_Combat/CombatStaminaSystem.h"
#include "Variant_Combat/CombatFlowSystem.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Variant_Combat/AI/CombatWaveSpawner.h"
#include "Variant_Combat/AI/CombatEnemy.h"
#include "Variant_Combat/CombatGameMode.h"

UCombatHUD::UCombatHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoUpdate = true;
	UpdateRate = 0.1f;
}

void UCombatHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// Try to find character automatically if not set
	if (!TargetCharacter)
	{
		if (APlayerController* PC = GetOwningPlayer())
		{
			if (APawn* Pawn = PC->GetPawn())
			{
				SetCharacter(Cast<ACombatCharacter>(Pawn));
			}
		}
	}
}

void UCombatHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bAutoUpdate)
	{
		LastUpdateTime += InDeltaTime;
		if (LastUpdateTime >= UpdateRate)
		{
			UpdateFromCharacter();
			LastUpdateTime = 0.0f;
		}
	}
}

void UCombatHUD::SetCharacter(ACombatCharacter* Character)
{
	TargetCharacter = Character;
	UpdateFromCharacter();
}

void UCombatHUD::UpdateHealth(float HealthPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(FMath::Clamp(HealthPercent, 0.0f, 1.0f));
	}
}

void UCombatHUD::UpdateStamina(float StaminaPercent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(FMath::Clamp(StaminaPercent, 0.0f, 1.0f));
	}
}

void UCombatHUD::UpdateFlow(float FlowPercent)
{
	if (FlowBar)
	{
		FlowBar->SetPercent(FMath::Clamp(FlowPercent, 0.0f, 1.0f));
	}
}

void UCombatHUD::UpdateWaveInfo(int32 CurrentWave, int32 EnemiesRemaining)
{
	if (WaveNumberText)
	{
		WaveNumberText->SetText(FText::FromString(FString::Printf(TEXT("Wave %d"), CurrentWave)));
	}

	if (EnemiesRemainingText)
	{
		EnemiesRemainingText->SetText(FText::FromString(FString::Printf(TEXT("Enemies: %d"), EnemiesRemaining)));
	}
}

void UCombatHUD::SetWaveInfoVisible(bool bVisible)
{
	if (WaveInfoPanel)
	{
		WaveInfoPanel->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UCombatHUD::UpdateFromCharacter()
{
	if (!TargetCharacter)
	{
		return;
	}

	// Update health
	float CurrentHP = TargetCharacter->GetCurrentHP();
	float MaxHP = TargetCharacter->GetMaxHP();
	if (MaxHP > 0.0f)
	{
		UpdateHealth(CurrentHP / MaxHP);
	}

	// Update stamina
	if (UCombatStaminaSystem* StaminaSystem = TargetCharacter->FindComponentByClass<UCombatStaminaSystem>())
	{
		UpdateStamina(StaminaSystem->GetStamina());
	}

	// Update flow
	if (UCombatFlowSystem* FlowSystem = TargetCharacter->FindComponentByClass<UCombatFlowSystem>())
	{
		UpdateFlow(FlowSystem->GetFlowMeter());
	}

	// Update wave info
	if (UWorld* World = GetWorld())
	{
		if (ACombatGameMode* GameMode = Cast<ACombatGameMode>(UGameplayStatics::GetGameMode(World)))
		{
			if (GameMode && GameMode->WaveSpawner)
			{
				ACombatWaveSpawner* WaveSpawner = GameMode->WaveSpawner;
				// Get current wave and enemy count
				TArray<ACombatEnemy*> CurrentEnemies = WaveSpawner->GetCurrentWaveEnemies();
				int32 WaveNumber = WaveSpawner->GetCurrentWaveIndex() + 1; // 1-indexed for display
				UpdateWaveInfo(WaveNumber, CurrentEnemies.Num());
				SetWaveInfoVisible(true);
			}
			else
			{
				SetWaveInfoVisible(false);
			}
		}
	}
}
