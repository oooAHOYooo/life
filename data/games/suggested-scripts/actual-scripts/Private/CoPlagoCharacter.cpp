// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoPlagoCharacter.h"
#include "CoPlagoEnemy.h"
#include "CoPlagoGameModeBase.h"
#include "CoPlago.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

static const FName CoPlagoLockOnTag(TEXT("CoPlagoLockOnTarget"));

ACoPlagoCharacter::ACoPlagoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	LockOnTarget = nullptr;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ACoPlagoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC)
	{
		UE_LOG(LogCoPlago, Warning, TEXT("CoPlago Character: Enhanced Input required. Assign Input Actions in Blueprint."));
		return;
	}

	if (JumpAction)
	{
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
	if (MoveAction)
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACoPlagoCharacter::Move);
	if (LookAction)
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACoPlagoCharacter::Look);
	if (MouseLookAction)
		EIC->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ACoPlagoCharacter::Look);
	if (LockOnAction)
	{
		EIC->BindAction(LockOnAction, ETriggerEvent::Started, this, &ACoPlagoCharacter::LockOnPressed);
		EIC->BindAction(LockOnAction, ETriggerEvent::Completed, this, &ACoPlagoCharacter::LockOnReleased);
	}
	if (AttackAction)
		EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACoPlagoCharacter::AttackTriggered);
}

void ACoPlagoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsDead) return;

	if (LockOnTarget && IsValid(LockOnTarget))
	{
		FVector ToTarget = LockOnTarget->GetActorLocation() - GetActorLocation();
		if (ToTarget.IsNearlyZero()) return;

		FRotator DesiredViewRot = ToTarget.Rotation();
		FRotator DesiredActorRot(0.f, DesiredViewRot.Yaw, 0.f);

		if (AController* C = GetController())
		{
			FRotator Current = C->GetControlRotation();
			FRotator NewView = FMath::RInterpConstantTo(Current, DesiredViewRot, DeltaTime, LockOnRotationRate);
			C->SetControlRotation(NewView);
		}

		FRotator ActorCurrent = GetActorRotation();
		SetActorRotation(FMath::RInterpConstantTo(ActorCurrent, DesiredActorRot, DeltaTime, LockOnRotationRate));
	}
	else if (LockOnTarget && !IsValid(LockOnTarget))
	{
		LockOnTarget = nullptr;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

AActor* ACoPlagoCharacter::FindNearestLockOnTarget() const
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FVector MyLoc = GetActorLocation();
	FVector Fwd = GetActorForwardVector();
	Fwd.Z = 0.f;
	if (!Fwd.Normalize()) return nullptr;

	AActor* Best = nullptr;
	float BestScore = -1.f;

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* A = *It;
		if (!A || A == this || !A->ActorHasTag(CoPlagoLockOnTag)) continue;

		FVector ToA = A->GetActorLocation() - MyLoc;
		float Dist = ToA.Size();
		if (Dist > LockOnRange || Dist < 1.f) continue;

		ToA.Z = 0.f;
		ToA.Normalize();
		float Dot = FVector::DotProduct(Fwd, ToA);
		if (Dot < 0.3f) continue;

		float Score = Dot / (Dist / LockOnRange + 0.1f);
		if (Score > BestScore) { BestScore = Score; Best = A; }
	}
	return Best;
}

void ACoPlagoCharacter::DoLockOn()
{
	if (LockOnTarget && IsValid(LockOnTarget)) return;
	LockOnTarget = FindNearestLockOnTarget();
	if (LockOnTarget)
		GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ACoPlagoCharacter::DoUnlockOn()
{
	LockOnTarget = nullptr;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACoPlagoCharacter::LockOnPressed(const FInputActionValue& Value)
{
	DoLockOn();
}

void ACoPlagoCharacter::LockOnReleased(const FInputActionValue& Value)
{
	DoUnlockOn();
}

void ACoPlagoCharacter::AttackTriggered(const FInputActionValue& Value)
{
	DoMeleeAttack();
}

void ACoPlagoCharacter::DoMeleeAttack()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector End = Start + GetActorForwardVector() * MeleeRange;
	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	World->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(MeleeRadius), Params);

	TSet<ACoPlagoEnemy*> Damaged;
	for (const FHitResult& Hit : Hits)
	{
		ACoPlagoEnemy* Enemy = Cast<ACoPlagoEnemy>(Hit.GetActor());
		if (Enemy && !Damaged.Contains(Enemy))
		{
			Damaged.Add(Enemy);
			Enemy->CoPlagoTakeDamage(MeleeDamage);
		}
	}
}

void ACoPlagoCharacter::Move(const FInputActionValue& Value)
{
	FVector2D V = Value.Get<FVector2D>();
	DoMove(V.X, V.Y);
}

void ACoPlagoCharacter::Look(const FInputActionValue& Value)
{
	FVector2D V = Value.Get<FVector2D>();
	DoLook(V.X, V.Y);
}

void ACoPlagoCharacter::DoMove(float Right, float Forward)
{
	if (AController* C = GetController())
	{
		const FRotator R = C->GetControlRotation();
		const FRotator YawOnly(0, R.Yaw, 0);
		const FVector Fwd = FRotationMatrix(YawOnly).GetUnitAxis(EAxis::X);
		const FVector RightVec = FRotationMatrix(YawOnly).GetUnitAxis(EAxis::Y);
		AddMovementInput(Fwd, Forward);
		AddMovementInput(RightVec, Right);
	}
}

void ACoPlagoCharacter::DoLook(float Yaw, float Pitch)
{
	if (AController* C = GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ACoPlagoCharacter::DoJumpStart()
{
	Jump();
}

void ACoPlagoCharacter::DoJumpEnd()
{
	StopJumping();
}

bool ACoPlagoCharacter::CoPlagoTakeDamage(float Damage)
{
	if (bIsDead) return true;
	Health = FMath::Max(0.f, Health - Damage);
	if (Health <= 0.f)
	{
		bIsDead = true;
		OnPlayerDeath();
		DisableInput(Cast<APlayerController>(GetController()));
		GetCharacterMovement()->DisableMovement();
		AController* C = GetController();
		ACoPlagoGameModeBase* P2GM = GetWorld() ? Cast<ACoPlagoGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
		if (P2GM && C)
			P2GM->RequestRespawn(C, RespawnDelay);
		return true;
	}
	return false;
}

void ACoPlagoCharacter::OnPlayerDeath_Implementation()
{
}
