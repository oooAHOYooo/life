// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoPlagoEnemy.h"
#include "CoPlagoEnemyWaveSpawner.h"
#include "CoPlagoCharacter.h"
#include "CoPlago.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"
#include "Components/CapsuleComponent.h"

static const FName CoPlagoLockOnTag(TEXT("CoPlagoLockOnTarget"));

ACoPlagoEnemy::ACoPlagoEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(CoPlagoLockOnTag);

	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	Health = MaxHealth;
}

void ACoPlagoEnemy::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

void ACoPlagoEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Health > 0.f)
	{
		ChasePlayer(DeltaTime);
		TryContactDamage(GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f);
	}
}

void ACoPlagoEnemy::ChasePlayer(float DeltaTime)
{
	UWorld* World = GetWorld();
	if (!World) return;

	APawn* Player = UGameplayStatics::GetPlayerPawn(World, 0);
	if (!Player) return;

	FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
	ToPlayer.Z = 0.f;
	float Dist = ToPlayer.Size();
	if (Dist < StopDistance) return;

	ToPlayer.Normalize();
	AddMovementInput(ToPlayer, 1.f);
}

void ACoPlagoEnemy::SetWaveSpawner(ACoPlagoEnemyWaveSpawner* Spawner)
{
	WaveSpawner = Spawner;
}

bool ACoPlagoEnemy::CoPlagoTakeDamage(float Damage)
{
	if (Health <= 0.f) return true;
	Health = FMath::Max(0.f, Health - Damage);
	if (Health <= 0.f)
	{
		OnEnemyDeath();
		if (ACoPlagoEnemyWaveSpawner* Sp = WaveSpawner.Get())
			Sp->NotifyEnemyDied(this);
		Destroy();
		return true;
	}
	return false;
}

void ACoPlagoEnemy::OnEnemyDeath_Implementation()
{
}

void ACoPlagoEnemy::TryContactDamage(float CurrentTime)
{
	if (CurrentTime - LastContactDamageTime < ContactDamageInterval) return;

	UWorld* World = GetWorld();
	if (!World) return;

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = World->OverlapMultiByChannel(Overlaps, GetActorLocation(), FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(ContactDamageRadius), Params);

	for (const FOverlapResult& R : Overlaps)
	{
		ACoPlagoCharacter* Char = Cast<ACoPlagoCharacter>(R.GetActor());
		if (Char && !Char->bIsDead)
		{
			Char->CoPlagoTakeDamage(ContactDamage);
			LastContactDamageTime = CurrentTime;
			return;
		}
	}
}
