// Copyright Epic Games, Inc. All Rights Reserved.

#include "P2G4WEnemy.h"
#include "P2G4WEnemyWaveSpawner.h"
#include "P2G4WCharacter.h"
#include "P2G4W.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"
#include "Components/CapsuleComponent.h"

static const FName P2G4WLockOnTag(TEXT("P2G4WLockOnTarget"));

AP2G4WEnemy::AP2G4WEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(P2G4WLockOnTag);

	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	Health = MaxHealth;
}

void AP2G4WEnemy::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

void AP2G4WEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Health > 0.f)
	{
		ChasePlayer(DeltaTime);
		TryContactDamage(GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f);
	}
}

void AP2G4WEnemy::ChasePlayer(float DeltaTime)
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

void AP2G4WEnemy::SetWaveSpawner(AP2G4WEnemyWaveSpawner* Spawner)
{
	WaveSpawner = Spawner;
}

bool AP2G4WEnemy::P2G4WTakeDamage(float Damage)
{
	if (Health <= 0.f) return true;
	Health = FMath::Max(0.f, Health - Damage);
	if (Health <= 0.f)
	{
		OnEnemyDeath();
		if (AP2G4WEnemyWaveSpawner* Sp = WaveSpawner.Get())
			Sp->NotifyEnemyDied(this);
		Destroy();
		return true;
	}
	return false;
}

void AP2G4WEnemy::OnEnemyDeath_Implementation()
{
}

void AP2G4WEnemy::TryContactDamage(float CurrentTime)
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
		AP2G4WCharacter* Char = Cast<AP2G4WCharacter>(R.GetActor());
		if (Char && !Char->bIsDead)
		{
			Char->P2G4WTakeDamage(ContactDamage);
			LastContactDamageTime = CurrentTime;
			return;
		}
	}
}
