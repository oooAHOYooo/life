// Copyright Epic Games, Inc. All Rights Reserved.

#include "P2G4WCharacter.h"
#include "P2G4WEnemy.h"
#include "P2G4WGameModeBase.h"
#include "P2G4W.h"
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

static const FName P2G4WLockOnTag(TEXT("P2G4WLockOnTarget"));

AP2G4WCharacter::AP2G4WCharacter()
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

void AP2G4WCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC)
	{
		UE_LOG(LogP2G4W, Warning, TEXT("P2G4W Character: Enhanced Input required. Assign Input Actions in Blueprint."));
		return;
	}

	if (JumpAction)
	{
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
	if (MoveAction)
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AP2G4WCharacter::Move);
	if (LookAction)
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AP2G4WCharacter::Look);
	if (MouseLookAction)
		EIC->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AP2G4WCharacter::Look);
	if (LockOnAction)
	{
		EIC->BindAction(LockOnAction, ETriggerEvent::Started, this, &AP2G4WCharacter::LockOnPressed);
		EIC->BindAction(LockOnAction, ETriggerEvent::Completed, this, &AP2G4WCharacter::LockOnReleased);
	}
	if (AttackAction)
		EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AP2G4WCharacter::AttackTriggered);
}

void AP2G4WCharacter::Tick(float DeltaTime)
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

AActor* AP2G4WCharacter::FindNearestLockOnTarget() const
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
		if (!A || A == this || !A->ActorHasTag(P2G4WLockOnTag)) continue;

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

void AP2G4WCharacter::DoLockOn()
{
	if (LockOnTarget && IsValid(LockOnTarget)) return;
	LockOnTarget = FindNearestLockOnTarget();
	if (LockOnTarget)
		GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AP2G4WCharacter::DoUnlockOn()
{
	LockOnTarget = nullptr;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AP2G4WCharacter::LockOnPressed(const FInputActionValue& Value)
{
	DoLockOn();
}

void AP2G4WCharacter::LockOnReleased(const FInputActionValue& Value)
{
	DoUnlockOn();
}

void AP2G4WCharacter::AttackTriggered(const FInputActionValue& Value)
{
	DoMeleeAttack();
}

void AP2G4WCharacter::DoMeleeAttack()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector End = Start + GetActorForwardVector() * MeleeRange;
	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	World->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(MeleeRadius), Params);

	TSet<AP2G4WEnemy*> Damaged;
	for (const FHitResult& Hit : Hits)
	{
		AP2G4WEnemy* Enemy = Cast<AP2G4WEnemy>(Hit.GetActor());
		if (Enemy && !Damaged.Contains(Enemy))
		{
			Damaged.Add(Enemy);
			Enemy->P2G4WTakeDamage(MeleeDamage);
		}
	}
}

void AP2G4WCharacter::Move(const FInputActionValue& Value)
{
	FVector2D V = Value.Get<FVector2D>();
	DoMove(V.X, V.Y);
}

void AP2G4WCharacter::Look(const FInputActionValue& Value)
{
	FVector2D V = Value.Get<FVector2D>();
	DoLook(V.X, V.Y);
}

void AP2G4WCharacter::DoMove(float Right, float Forward)
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

void AP2G4WCharacter::DoLook(float Yaw, float Pitch)
{
	if (AController* C = GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AP2G4WCharacter::DoJumpStart()
{
	Jump();
}

void AP2G4WCharacter::DoJumpEnd()
{
	StopJumping();
}

bool AP2G4WCharacter::P2G4WTakeDamage(float Damage)
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
		AP2G4WGameModeBase* P2GM = GetWorld() ? Cast<AP2G4WGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
		if (P2GM && C)
			P2GM->RequestRespawn(C, RespawnDelay);
		return true;
	}
	return false;
}

void AP2G4WCharacter::OnPlayerDeath_Implementation()
{
}
