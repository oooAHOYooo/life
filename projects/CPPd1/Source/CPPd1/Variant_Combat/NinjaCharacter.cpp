// Copyright Epic Games, Inc. All Rights Reserved.

#include "NinjaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "Variant_Combat/CombatCharacter.h"
#include "CPPd1LockOnTargetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

ANinjaCharacter::ANinjaCharacter()
{
	MovementSpeed = 400.0f;
	RotationRate = 540.0f;
	LockOnRotationSpeed = 450.0f;
	LockOnRadius = 1800.0f;
	DefaultCameraDistance = 300.0f;
}

void ANinjaCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Set jump count - JumpMaxCount is a property of Character, not CharacterMovementComponent
	JumpMaxCount = FMath::Max(1, JumpCountMax);

	// Start at relaxed walk speed
	GetCharacterMovement()->MaxWalkSpeed = RelaxedWalkSpeed;
}

void ANinjaCharacter::DoRoll()
{
	if (bIsRolling || bIsAttacking) return;

	if (LockOnTarget)
	{
		UCPPd1LockOnTargetComponent* Comp = LockOnTarget->FindComponentByClass<UCPPd1LockOnTargetComponent>();
		if (Comp)
		{
			FVector ToTarget = (Comp->GetLockOnWorldLocation() - GetActorLocation()).GetSafeNormal2D();
			if (!ToTarget.IsNearlyZero())
			{
				RollDirection = (-ToTarget).GetSafeNormal2D();
				RollDirection.Z = 0.f;
			}
			else
			{
				RollDirection = -GetActorForwardVector();
				RollDirection.Z = 0.f;
				RollDirection.Normalize();
			}
		}
		else
		{
			RollDirection = -GetActorForwardVector();
			RollDirection.Z = 0.f;
			RollDirection.Normalize();
		}
	}
	else
	{
		FVector LastInput = GetCharacterMovement()->GetLastInputVector();
		if (LastInput.IsNearlyZero())
			RollDirection = -GetActorForwardVector();
		else
			RollDirection = LastInput.GetSafeNormal2D();
		RollDirection.Z = 0.f;
		if (!RollDirection.Normalize())
			RollDirection = -GetActorForwardVector();
		RollDirection.Z = 0.f;
	}

	RollTimeRemaining = RollDuration;
	bIsRolling = true;

	if (GhostCharacter)
	{
		ANinjaCharacter* NinjaGhost = Cast<ANinjaCharacter>(GhostCharacter);
		if (NinjaGhost)
		{
			NinjaGhost->RollDirection = RollDirection;
			NinjaGhost->RollTimeRemaining = RollDuration;
			NinjaGhost->bIsRolling = true;
		}
	}
}

void ANinjaCharacter::DoBackflip()
{
	if (bIsFlipping || bIsRolling) return;
	StartFlip(ENinjaFlipType::Backflip, -GetActorForwardVector());
}

void ANinjaCharacter::DoFrontflip()
{
	if (bIsFlipping || bIsRolling) return;
	StartFlip(ENinjaFlipType::Frontflip, GetActorForwardVector());
}

void ANinjaCharacter::DoSideflipLeft()
{
	if (bIsFlipping || bIsRolling) return;
	StartFlip(ENinjaFlipType::SideflipLeft, -GetActorRightVector());
}

void ANinjaCharacter::DoSideflipRight()
{
	if (bIsFlipping || bIsRolling) return;
	StartFlip(ENinjaFlipType::SideflipRight, GetActorRightVector());
}

void ANinjaCharacter::DoFlip360()
{
	if (bIsFlipping || bIsRolling) return;
	StartFlip(ENinjaFlipType::Flip360, FVector::Zero());
}

void ANinjaCharacter::StartFlip(ENinjaFlipType FlipType, const FVector& HorizontalDir)
{
	bIsFlipping = true;
	CurrentFlipType = FlipType;
	FlipElapsed = 0.0f;
	MeshFlipBaseRotation = GetMesh()->GetRelativeRotation();

	switch (FlipType)
	{
	case ENinjaFlipType::Backflip:
	case ENinjaFlipType::Frontflip:
	case ENinjaFlipType::Flip360:
		FlipTotalDegrees = (FlipType == ENinjaFlipType::Flip360) ? 360.0f : 180.0f;
		break;
	case ENinjaFlipType::SideflipLeft:
	case ENinjaFlipType::SideflipRight:
		FlipTotalDegrees = 180.0f;
		break;
	default:
		FlipTotalDegrees = 180.0f;
		break;
	}

	FVector Impulse = FVector::UpVector * FlipUpImpulse;
	if (!HorizontalDir.IsNearlyZero())
		Impulse += HorizontalDir.GetSafeNormal() * FlipHorizontalImpulse;
	LaunchCharacter(Impulse, true, true);

	if (GhostCharacter)
	{
		ANinjaCharacter* G = Cast<ANinjaCharacter>(GhostCharacter);
		if (G && !G->bIsFlipping)
		{
			G->bIsFlipping = true;
			G->CurrentFlipType = FlipType;
			G->FlipElapsed = 0.0f;
			G->MeshFlipBaseRotation = G->GetMesh()->GetRelativeRotation();
			G->FlipTotalDegrees = FlipTotalDegrees;
			G->LaunchCharacter(Impulse, true, true);
		}
	}
}

void ANinjaCharacter::UpdateFlipMeshRotation(float DeltaTime)
{
	FlipElapsed += DeltaTime;
	const float T = FMath::Clamp(FlipElapsed / FlipDuration, 0.0f, 1.0f);
	const float AngleDeg = FlipTotalDegrees * T;

	float PitchDelta = 0.0f, RollDelta = 0.0f;
	switch (CurrentFlipType)
	{
	case ENinjaFlipType::Backflip:
		PitchDelta = AngleDeg;
		break;
	case ENinjaFlipType::Frontflip:
		PitchDelta = -AngleDeg;
		break;
	case ENinjaFlipType::SideflipLeft:
		RollDelta = AngleDeg;
		break;
	case ENinjaFlipType::SideflipRight:
		RollDelta = -AngleDeg;
		break;
	case ENinjaFlipType::Flip360:
		PitchDelta = AngleDeg;
		break;
	default:
		break;
	}

	FRotator DeltaRot(PitchDelta, 0.0f, RollDelta);
	GetMesh()->SetRelativeRotation(MeshFlipBaseRotation + DeltaRot);
}

void ANinjaCharacter::EndFlip()
{
	bIsFlipping = false;
	CurrentFlipType = ENinjaFlipType::None;
	GetMesh()->SetRelativeRotation(MeshFlipBaseRotation);
	if (GhostCharacter)
	{
		ANinjaCharacter* G = Cast<ANinjaCharacter>(GhostCharacter);
		if (G) G->EndFlip();
	}
}

void ANinjaCharacter::DoKick()
{
	if (bIsAttacking || !KickMontage) return;
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	if (Anim)
		Anim->Montage_Play(KickMontage, 1.0f);
	if (GhostCharacter)
	{
		ANinjaCharacter* G = Cast<ANinjaCharacter>(GhostCharacter);
		if (G && G->KickMontage) G->DoKick();
	}
}

bool ANinjaCharacter::ConsumeLastHitWasDuringFlip()
{
	const bool b = bLastDealtDamageWhileFlipping;
	bLastDealtDamageWhileFlipping = false;
	return b;
}

void ANinjaCharacter::DoAttackTrace(FName DamageSourceBone)
{
	bLastDealtDamageWhileFlipping = bIsFlipping;
	Super::DoAttackTrace(DamageSourceBone);
}

void ANinjaCharacter::Tick(float DeltaTime)
{
	// Movement Speed Logic (Sprint when locked on, Walk when not)
	if (!bIsFlying)
	{
		if (LockOnTarget != nullptr)
		{
			GetCharacterMovement()->MaxWalkSpeed = CombatSprintSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = RelaxedWalkSpeed;
		}
	}

	if (bIsRolling)
	{
		RollTimeRemaining -= DeltaTime;
		FVector Vel = RollDirection * RollSpeed;
		Vel.Z = GetCharacterMovement()->Velocity.Z;
		GetCharacterMovement()->Velocity = Vel;
		if (RollTimeRemaining <= 0.0f)
		{
			bIsRolling = false;
			if (GhostCharacter)
			{
				ANinjaCharacter* G = Cast<ANinjaCharacter>(GhostCharacter);
				if (G) G->bIsRolling = false;
			}
		}
	}
	else if (bIsFlipping)
	{
		UpdateFlipMeshRotation(DeltaTime);
		if (FlipElapsed >= FlipDuration)
			EndFlip();
	}
	else
	{
		Super::Tick(DeltaTime);
	}
}

void ANinjaCharacter::ToggleFlight()
{
	bIsFlying = !bIsFlying;

	if (bIsFlying)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->MaxFlySpeed = CombatSprintSpeed * 2.0f; // DBZ fly fast
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	if (GhostCharacter)
	{
		ANinjaCharacter* G = Cast<ANinjaCharacter>(GhostCharacter);
		if (G)
		{
			G->bIsFlying = bIsFlying;
			if (bIsFlying) G->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			else G->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}
}

void ANinjaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC) return;

	if (RollAction)
		EIC->BindAction(RollAction, ETriggerEvent::Started, this, &ANinjaCharacter::DoRoll);
	if (BackflipAction)
		EIC->BindAction(BackflipAction, ETriggerEvent::Started, this, &ANinjaCharacter::DoBackflip);
	if (FrontflipAction)
		EIC->BindAction(FrontflipAction, ETriggerEvent::Started, this, &ANinjaCharacter::DoFrontflip);
	if (SideflipLeftAction)
		EIC->BindAction(SideflipLeftAction, ETriggerEvent::Started, this, &ANinjaCharacter::DoSideflipLeft);
	if (SideflipRightAction)
		EIC->BindAction(SideflipRightAction, ETriggerEvent::Started, this, &ANinjaCharacter::DoSideflipRight);
	if (Flip360Action)
		EIC->BindAction(Flip360Action, ETriggerEvent::Started, this, &ANinjaCharacter::DoFlip360);
	if (KickAction)
		EIC->BindAction(KickAction, ETriggerEvent::Started, this, &ANinjaCharacter::DoKick);
	if (FlightAction)
		EIC->BindAction(FlightAction, ETriggerEvent::Started, this, &ANinjaCharacter::ToggleFlight);
}
