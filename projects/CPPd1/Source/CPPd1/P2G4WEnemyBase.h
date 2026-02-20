// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P2G4WEnemyBase.generated.h"

UCLASS(Abstract)
class CPPD1_API AP2G4WEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AP2G4WEnemyBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Health and damage event stubs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void TakeDamageFromGod(float DamageAmount, FVector HitLocation);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void OnDeath();
};
