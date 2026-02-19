// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPo1.h"
#include "Components/ActorComponent.h"
#include "CombatEngagementManager.generated.h"

class ACombatEnemy;
class ACombatCharacter;

/**
 * Manages sequential enemy engagement - only one enemy fights at a time (UFC-style)
 */
UCLASS(ClassGroup = (Combat), meta = (BlueprintSpawnableComponent))
class CPPo1_API UCombatEngagementManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatEngagementManager();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Register an enemy to the engagement queue */
	UFUNCTION(BlueprintCallable, Category = "Engagement")
	void RegisterEnemy(ACombatEnemy* Enemy);

	/** Register multiple enemies at once */
	UFUNCTION(BlueprintCallable, Category = "Engagement")
	void RegisterEnemies(const TArray<ACombatEnemy*>& Enemies);

	/** Start engagement with the next enemy in queue */
	UFUNCTION(BlueprintCallable, Category = "Engagement")
	void StartNextEngagement();

	/** End current engagement and move to next */
	UFUNCTION(BlueprintCallable, Category = "Engagement")
	void EndCurrentEngagement();

	/** Get the currently engaged enemy */
	UFUNCTION(BlueprintPure, Category = "Engagement")
	ACombatEnemy* GetCurrentEnemy() const { return CurrentEnemy; }

	/** Get all enemies waiting to engage */
	UFUNCTION(BlueprintPure, Category = "Engagement")
	TArray<ACombatEnemy*> GetWaitingEnemies() const;

	/** Check if an engagement is active */
	UFUNCTION(BlueprintPure, Category = "Engagement")
	bool IsEngagementActive() const { return CurrentEnemy != nullptr; }

	/** Clear all enemies from queue */
	UFUNCTION(BlueprintCallable, Category = "Engagement")
	void ClearQueue();

protected:
	/** Queue of enemies waiting to engage */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Engagement")
	TArray<TObjectPtr<ACombatEnemy>> EnemyQueue;

	/** Currently engaged enemy */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Engagement")
	TObjectPtr<ACombatEnemy> CurrentEnemy;

	/** Player characters (for targeting) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Engagement")
	TArray<TObjectPtr<ACombatCharacter>> PlayerCharacters;

	/** Distance threshold for engagement (enemy must be within this to engage) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engagement", meta = (ClampMin = 0.0f, Units = "cm"))
	float EngagementDistance = 500.0f;

	/** Time delay before starting next engagement after current ends */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engagement", meta = (ClampMin = 0.0f, Units = "s"))
	float EngagementTransitionDelay = 2.0f;

	/** Enable/disable AI for enemies not currently engaged */
	UFUNCTION(BlueprintCallable, Category = "Engagement")
	void SetEnemyAIEnabled(ACombatEnemy* Enemy, bool bEnabled);

	/** Called when engagement starts (internal handler) */
	void HandleEngagementStarted(ACombatEnemy* Enemy);

	/** Called when engagement ends (internal handler) */
	void HandleEngagementEnded(ACombatEnemy* Enemy);

	FTimerHandle EngagementTimer;

public:
	/** Delegate for when engagement starts */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEngagementStarted, ACombatEnemy*, Enemy);

	/** Delegate for when engagement ends */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEngagementEnded, ACombatEnemy*, Enemy);

	/** Event fired when engagement starts */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEngagementStarted OnEngagementStarted;

	/** Event fired when engagement ends */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEngagementEnded OnEngagementEnded;
};
