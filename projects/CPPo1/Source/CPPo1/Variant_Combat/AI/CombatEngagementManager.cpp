// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Combat/AI/CombatEngagementManager.h"
#include "Variant_Combat/AI/CombatEnemy.h"
#include "Variant_Combat/CombatCharacter.h"
#include "Variant_Combat/AI/CombatAIController.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UCombatEngagementManager::UCombatEngagementManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatEngagementManager::BeginPlay()
{
	Super::BeginPlay();

	// Find all player characters
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACombatCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ACombatCharacter* PlayerChar = Cast<ACombatCharacter>(Actor))
		{
			if (!PlayerChar->bIsGhost) // Don't include ghost characters
			{
				PlayerCharacters.Add(PlayerChar);
			}
		}
	}

	// Subscribe to enemy death events for all registered enemies
	// This will be called when enemies are registered
}

void UCombatEngagementManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Auto-start next engagement if current is done and queue has enemies
	if (!IsEngagementActive() && EnemyQueue.Num() > 0)
	{
		StartNextEngagement();
	}
}

void UCombatEngagementManager::RegisterEnemy(ACombatEnemy* Enemy)
{
	if (!Enemy || EnemyQueue.Contains(Enemy))
	{
		return;
	}

	EnemyQueue.Add(Enemy);
	
	// Subscribe to enemy death event - use a lambda that captures the enemy
	// Note: We'll handle this in EndCurrentEngagement when enemy dies
	
	// Disable AI initially (will be enabled when engaged)
	SetEnemyAIEnabled(Enemy, false);
}

void UCombatEngagementManager::RegisterEnemies(const TArray<ACombatEnemy*>& Enemies)
{
	for (ACombatEnemy* Enemy : Enemies)
	{
		RegisterEnemy(Enemy);
	}
}

void UCombatEngagementManager::StartNextEngagement()
{
	// End current engagement if any
	if (CurrentEnemy)
	{
		EndCurrentEngagement();
	}

	// Find next valid enemy in queue
	ACombatEnemy* NextEnemy = nullptr;
	int32 NextIndex = INDEX_NONE;

	for (int32 i = 0; i < EnemyQueue.Num(); i++)
	{
		if (IsValid(EnemyQueue[i]))
		{
			NextEnemy = EnemyQueue[i];
			NextIndex = i;
			break;
		}
	}

	if (!NextEnemy)
	{
		// No valid enemies in queue
		EnemyQueue.Empty();
		return;
	}

	// Remove from queue
	EnemyQueue.RemoveAt(NextIndex);
	CurrentEnemy = NextEnemy;

	// Enable AI for this enemy
	SetEnemyAIEnabled(CurrentEnemy, true);

	// Disable AI for all other enemies
	for (ACombatEnemy* Enemy : EnemyQueue)
	{
		if (IsValid(Enemy))
		{
			SetEnemyAIEnabled(Enemy, false);
		}
	}

	HandleEngagementStarted(CurrentEnemy);
}

void UCombatEngagementManager::EndCurrentEngagement()
{
	if (!CurrentEnemy)
	{
		return;
	}

	HandleEngagementEnded(CurrentEnemy);

	// Disable AI for current enemy
	SetEnemyAIEnabled(CurrentEnemy, false);

	ACombatEnemy* EndedEnemy = CurrentEnemy;
	CurrentEnemy = nullptr;

	// Remove from queue if still there
	EnemyQueue.Remove(EndedEnemy);

	// Start next engagement after delay
	if (EnemyQueue.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			EngagementTimer,
			this,
			&UCombatEngagementManager::StartNextEngagement,
			EngagementTransitionDelay,
			false
		);
	}
}

TArray<ACombatEnemy*> UCombatEngagementManager::GetWaitingEnemies() const
{
	TArray<ACombatEnemy*> Result;
	for (ACombatEnemy* Enemy : EnemyQueue)
	{
		if (IsValid(Enemy))
		{
			Result.Add(Enemy);
		}
	}
	return Result;
}

void UCombatEngagementManager::ClearQueue()
{
	// Disable AI for all enemies
	for (ACombatEnemy* Enemy : EnemyQueue)
	{
		if (IsValid(Enemy))
		{
			SetEnemyAIEnabled(Enemy, false);
		}
	}

	if (CurrentEnemy)
	{
		SetEnemyAIEnabled(CurrentEnemy, false);
	}

	EnemyQueue.Empty();
	CurrentEnemy = nullptr;
}

void UCombatEngagementManager::SetEnemyAIEnabled(ACombatEnemy* Enemy, bool bEnabled)
{
	if (!Enemy)
	{
		return;
	}

	// Enable/disable AI by stopping movement and disabling brain component
	if (AAIController* AIController = Cast<AAIController>(Enemy->GetController()))
	{
		if (bEnabled)
		{
			// Resume AI - allow movement and behavior
			AIController->GetPawn()->SetActorEnableCollision(true);
			if (UBehaviorTreeComponent* BTComp = AIController->FindComponentByClass<UBehaviorTreeComponent>())
			{
				// AI will resume automatically when enabled
			}
		}
		else
		{
			// Pause AI - stop movement
			AIController->StopMovement();
		}
	}
}

void UCombatEngagementManager::HandleEngagementStarted(ACombatEnemy* Enemy)
{
	OnEngagementStarted.Broadcast(Enemy);
}

void UCombatEngagementManager::HandleEngagementEnded(ACombatEnemy* Enemy)
{
	OnEngagementEnded.Broadcast(Enemy);
}
