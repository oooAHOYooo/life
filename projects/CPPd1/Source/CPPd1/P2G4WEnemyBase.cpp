// Fill out your copyright notice in the Description page of Project Settings.


#include "P2G4WEnemyBase.h"

AP2G4WEnemyBase::AP2G4WEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
}

void AP2G4WEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void AP2G4WEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP2G4WEnemyBase::TakeDamageFromGod(float DamageAmount, FVector HitLocation)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0)
	{
		OnDeath();
		Destroy(); // Basic death for now
	}
}
