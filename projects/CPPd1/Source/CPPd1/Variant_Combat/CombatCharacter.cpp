// Copyright Epic Games, Inc. All Rights Reserved.


#include "CombatCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CombatLifeBar.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"
#include "Engine/LocalPlayer.h"
#include "CombatPlayerController.h"
#include "CPPd1LockOnTargetComponent.h"
#include "CombatStaminaSystem.h"
#include "CombatFlowSystem.h"
#include "CombatAdvancedMechanics.h"
#include "CombatSkillSystem.h"

ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// bind the attack montage ended delegate
	OnAttackMontageEnded.BindUObject(this, &ACombatCharacter::AttackMontageEnded);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.0f, 90.0f);

	// Configure character movement (will be overridden by tuning variables in BeginPlay)
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->AirControl = AirControl;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, RotationRate, 0.0f);

	// create the camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = DefaultCameraDistance;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	// create the orbiting camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// create the life bar widget component
	LifeBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("LifeBar"));
	LifeBar->SetupAttachment(RootComponent);

	// set the player tag
	Tags.Add(FName("Player"));
}

void ACombatCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ACombatCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ACombatCharacter::ComboAttackPressed()
{
	// route the input
	DoComboAttackStart();
}

void ACombatCharacter::ChargedAttackPressed()
{
	// route the input
	DoChargedAttackStart();
}

void ACombatCharacter::ChargedAttackReleased()
{
	// route the input
	DoChargedAttackEnd();
}

void ACombatCharacter::LockOnPressed()
{
	if (LockOnTarget)
	{
		DoClearLockOn();
	}
	else
	{
		DoLockOn();
	}
}

void ACombatCharacter::DoLockOn()
{
	TArray<AActor*> Targets;
	UCPPd1LockOnTargetComponent::FindLockOnTargetsInRadius(GetWorld(), GetActorLocation(), LockOnRadius, Targets);
	LockOnTarget = Targets.Num() > 0 ? Targets[0] : nullptr;
	if (GhostCharacter) GhostCharacter->DoLockOn();
}

void ACombatCharacter::DoClearLockOn()
{
	LockOnTarget = nullptr;
	if (GhostCharacter) GhostCharacter->DoClearLockOn();
}

void ACombatCharacter::DoCycleLockOn()
{
	TArray<AActor*> Targets;
	UCPPd1LockOnTargetComponent::FindLockOnTargetsInRadius(GetWorld(), GetActorLocation(), LockOnRadius, Targets);
	if (Targets.Num() == 0)
	{
		LockOnTarget = nullptr;
		if (GhostCharacter) GhostCharacter->DoCycleLockOn();
		return;
	}
	int32 Index = Targets.IndexOfByKey(LockOnTarget);
	if (Index == INDEX_NONE) Index = -1;
	Index = (Index + 1) % Targets.Num();
	LockOnTarget = Targets[Index];
	if (GhostCharacter) GhostCharacter->DoCycleLockOn();
}

void ACombatCharacter::SetGhostCharacter(ACombatCharacter* Ghost)
{
	GhostCharacter = Ghost;
}

FRotator ACombatCharacter::GetEffectiveControlRotation() const
{
	if (GetController())
	{
		return GetController()->GetControlRotation();
	}
	return StoredControlRotation;
}

void ACombatCharacter::DoMove(float Right, float Forward)
{
	const FRotator Rotation = GetEffectiveControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);

	if (GhostCharacter)
	{
		GhostCharacter->StoredControlRotation = Rotation;
		GhostCharacter->DoMove(Right, Forward);
	}
}

void ACombatCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
	else
	{
		StoredControlRotation.Yaw += Yaw;
		StoredControlRotation.Pitch += Pitch;
		StoredControlRotation.Pitch = FMath::Clamp(StoredControlRotation.Pitch, -89.f, 89.f);
	}
	if (GhostCharacter)
	{
		GhostCharacter->StoredControlRotation = GetEffectiveControlRotation();
		GhostCharacter->DoLook(Yaw, Pitch);
	}
}

void ACombatCharacter::DoComboAttackStart()
{
	// are we already playing an attack animation?
	if (bIsAttacking)
	{
		// cache the input time so we can check it later
		CachedAttackInputTime = GetWorld()->GetTimeSeconds();

		return;
	}

	// perform a combo attack
	ComboAttack();
	if (GhostCharacter) GhostCharacter->DoComboAttackStart();
}

void ACombatCharacter::DoComboAttackEnd()
{
	if (GhostCharacter) GhostCharacter->DoComboAttackEnd();
}

void ACombatCharacter::DoChargedAttackStart()
{
	// raise the charging attack flag
	bIsChargingAttack = true;

	if (GhostCharacter) GhostCharacter->DoChargedAttackStart();

	if (bIsAttacking)
	{
		// cache the input time so we can check it later
		CachedAttackInputTime = GetWorld()->GetTimeSeconds();

		return;
	}

	ChargedAttack();
}

void ACombatCharacter::DoChargedAttackEnd()
{
	bIsChargingAttack = false;
	if (GhostCharacter) GhostCharacter->DoChargedAttackEnd();

	if (bHasLoopedChargedAttack)
	{
		CheckChargedAttack();
	}
}

void ACombatCharacter::ResetHP()
{
	// reset the current HP total
	CurrentHP = MaxHP;

	// update the life bar
	LifeBarWidget->SetLifePercentage(1.0f);
}

void ACombatCharacter::ComboAttack()
{
	// raise the attacking flag
	bIsAttacking = true;

	// reset the combo count
	ComboCount = 0;

	// play the attack montage
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float MontageLength = AnimInstance->Montage_Play(ComboAttackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

		// subscribe to montage completed and interrupted events
		if (MontageLength > 0.0f)
		{
			// set the end delegate for the montage
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, ComboAttackMontage);
		}
	}

}

void ACombatCharacter::ChargedAttack()
{
	// raise the attacking flag
	bIsAttacking = true;

	// reset the charge loop flag
	bHasLoopedChargedAttack = false;

	// play the charged attack montage
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float MontageLength = AnimInstance->Montage_Play(ChargedAttackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

		// subscribe to montage completed and interrupted events
		if (MontageLength > 0.0f)
		{
			// set the end delegate for the montage
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, ChargedAttackMontage);
		}
	}
}

void ACombatCharacter::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// reset the attacking flag
	bIsAttacking = false;

	// check if we have a non-stale cached input
	if (GetWorld()->GetTimeSeconds() - CachedAttackInputTime <= AttackInputCacheTimeTolerance)
	{
		// are we holding the charged attack button?
		if (bIsChargingAttack)
		{
			// do a charged attack
			ChargedAttack();
		}
		else
		{
			// do a regular attack
			ComboAttack();
		}
	}
}

void ACombatCharacter::DoAttackTrace(FName DamageSourceBone)
{
	// sweep for objects in front of the character to be hit by the attack
	TArray<FHitResult> OutHits;

	// start at the provided socket location, sweep forward
	const FVector TraceStart = GetMesh()->GetSocketLocation(DamageSourceBone);
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * MeleeTraceDistance);

	// check for pawn and world dynamic collision object types
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	// use a sphere shape for the sweep
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(MeleeTraceRadius);

	// ignore self
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, CollisionShape, QueryParams))
	{
		// iterate over each object hit
		for (const FHitResult& CurrentHit : OutHits)
		{
			// check if we've hit a damageable actor
			ICombatDamageable* Damageable = Cast<ICombatDamageable>(CurrentHit.GetActor());

			if (Damageable)
			{
				// knock upwards and away from the impact normal
				const FVector Impulse = (CurrentHit.ImpactNormal * -MeleeKnockbackImpulse) + (FVector::UpVector * MeleeLaunchImpulse);

				// Apply global damage multiplier
				float FinalDamage = MeleeDamage * GlobalDamageMultiplier;

				// pass the damage event to the actor
				Damageable->ApplyDamage(FinalDamage, this, CurrentHit.ImpactPoint, Impulse);

				// call the BP handler to play effects, etc.
				DealtDamage(FinalDamage, CurrentHit.ImpactPoint);
			}
		}
	}
}

void ACombatCharacter::CheckCombo()
{
	// are we playing a non-charge attack animation?
	if (bIsAttacking && !bIsChargingAttack)
	{
		// is the last attack input not stale?
		if (GetWorld()->GetTimeSeconds() - CachedAttackInputTime <= ComboInputCacheTimeTolerance)
		{
			// consume the attack input so we don't accidentally trigger it twice
			CachedAttackInputTime = 0.0f;

			// increase the combo counter
			++ComboCount;

			// do we still have a combo section to play?
			if (ComboCount < ComboSectionNames.Num())
			{
				// jump to the next combo section
				if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
				{
					AnimInstance->Montage_JumpToSection(ComboSectionNames[ComboCount], ComboAttackMontage);
				}
			}
		}
	}
}

void ACombatCharacter::CheckChargedAttack()
{
	// raise the looped charged attack flag
	bHasLoopedChargedAttack = true;

	// jump to either the loop or the attack section depending on whether we're still holding the charge button
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_JumpToSection(bIsChargingAttack ? ChargeLoopSection : ChargeAttackSection, ChargedAttackMontage);
	}
}

void ACombatCharacter::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
	// pass the damage event to the actor
	FDamageEvent DamageEvent;
	const float ActualDamage = TakeDamage(Damage, DamageEvent, nullptr, DamageCauser);

	// only process knockback and effects if we received nonzero damage
	if (ActualDamage > 0.0f)
	{
		// apply the knockback impulse
		GetCharacterMovement()->AddImpulse(DamageImpulse, true);

		// is the character ragdolling?
		if (GetMesh()->IsSimulatingPhysics())
		{
			// apply an impulse to the ragdoll
			GetMesh()->AddImpulseAtLocation(DamageImpulse * GetMesh()->GetMass(), DamageLocation);
		}

		// pass control to BP to play effects, etc.
		ReceivedDamage(ActualDamage, DamageLocation, DamageImpulse.GetSafeNormal());
	}

}

void ACombatCharacter::HandleDeath()
{
	// disable movement while we're dead
	GetCharacterMovement()->DisableMovement();

	// enable full ragdoll physics
	GetMesh()->SetSimulatePhysics(true);

	// hide the life bar
	LifeBar->SetHiddenInGame(true);

	// pull back the camera
	GetCameraBoom()->TargetArmLength = DeathCameraDistance;

	// schedule respawning
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ACombatCharacter::RespawnCharacter, RespawnTime, false);
}

void ACombatCharacter::ApplyHealing(float Healing, AActor* Healer)
{
	// stub
}

void ACombatCharacter::RespawnCharacter()
{
	// destroy the character and let it be respawned by the Player Controller
	Destroy();
}

float ACombatCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// ghosts don't take damage
	if (bIsGhost)
	{
		return 0.0f;
	}

	// Check invincibility frames
	if (TimeSinceLastDamage < InvincibilityFrames)
	{
		return 0.0f;
	}

	// Apply global defense multiplier
	Damage *= GlobalDefenseMultiplier;

	// only process damage if the character is still alive
	if (CurrentHP <= 0.0f)
	{
		return 0.0f;
	}

	// reduce the current HP
	CurrentHP -= Damage;

	// have we run out of HP?
	if (CurrentHP <= 0.0f)
	{
		// die
		HandleDeath();
	}
	else
	{
		// update the life bar
		LifeBarWidget->SetLifePercentage(CurrentHP / MaxHP);

		// enable partial ragdoll physics, but keep the pelvis vertical
		GetMesh()->SetPhysicsBlendWeight(0.5f);
		GetMesh()->SetBodySimulatePhysics(PelvisBoneName, false);
	}

	// Reset invincibility timer
	TimeSinceLastDamage = 0.0f;

	// return the received damage amount
	return Damage;
}

void ACombatCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// is the character still alive?
	if (CurrentHP >= 0.0f)
	{
		// disable ragdoll physics
		GetMesh()->SetPhysicsBlendWeight(0.0f);
	}
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Apply tuning variables to character movement
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->AirControl = AirControl;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, RotationRate, 0.0f);

	// get the life bar from the widget component
	LifeBarWidget = Cast<UCombatLifeBar>(LifeBar->GetUserWidgetObject());
	check(LifeBarWidget);

	// initialize the camera
	GetCameraBoom()->TargetArmLength = DefaultCameraDistance;

	// save the relative transform for the mesh so we can reset the ragdoll later
	MeshStartingTransform = GetMesh()->GetRelativeTransform();

	// set the life bar color
	LifeBarWidget->SetBarColor(LifeBarColor);

	// reset HP to maximum
	ResetHP();

	// Initialize tuning variables for component systems
	InitializeTuningVariables();

	if (bIsGhost)
	{
		if (LifeBar) LifeBar->SetHiddenInGame(true);
		if (CameraBoom) CameraBoom->SetHiddenInGame(true);
		if (FollowCamera) FollowCamera->SetHiddenInGame(true);
	}
}

void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update invincibility frames
	TimeSinceLastDamage += DeltaTime;

	// Keep ghost's control rotation in sync when we have a ghost (solo 2P)
	if (GhostCharacter && GetController())
	{
		GhostCharacter->StoredControlRotation = GetController()->GetControlRotation();
	}
	// Handle lock-on rotation
	if (LockOnTarget && CurrentHP > 0.0f && GetController())
	{
		UCPPd1LockOnTargetComponent* Comp = LockOnTarget->FindComponentByClass<UCPPd1LockOnTargetComponent>();
		if (Comp)
		{
			FVector TargetLoc = Comp->GetLockOnWorldLocation();
			FVector ToTarget = (TargetLoc - GetActorLocation()).GetSafeNormal2D();
			if (ToTarget.IsNearlyZero() == false)
			{
				FRotator DesiredYaw = ToTarget.Rotation();
				DesiredYaw.Pitch = 0.f;
				DesiredYaw.Roll = 0.f;
				FRotator Current = GetController()->GetControlRotation();
				Current.Pitch = 0.f;
				Current.Roll = 0.f;
				FRotator NewRot = FMath::RInterpTo(Current, DesiredYaw, DeltaTime, LockOnRotationSpeed / 360.f);
				GetController()->SetControlRotation(FRotator(GetController()->GetControlRotation().Pitch, NewRot.Yaw, NewRot.Roll));
			}
		}
		else
		{
			LockOnTarget = nullptr;
		}
	}
}

void ACombatCharacter::InitializeTuningVariables()
{
	// Initialize component systems with tuning variables if they exist
	// This allows the tuning variables to be set in the inspector and applied to components
	
	// Stamina System
	if (UCombatStaminaSystem* StaminaSystem = FindComponentByClass<UCombatStaminaSystem>())
	{
		// Note: These would need setters in the component, or we set them via reflection
		// For now, the component uses its own defaults, but you can override in Blueprint
	}

	// Flow System
	if (UCombatFlowSystem* FlowSystem = FindComponentByClass<UCombatFlowSystem>())
	{
		// Same note as above
	}

	// Advanced Mechanics
	if (UCombatAdvancedMechanics* AdvancedMech = FindComponentByClass<UCombatAdvancedMechanics>())
	{
		// Same note as above
	}

	// Skill System
	if (UCombatSkillSystem* SkillSystem = FindComponentByClass<UCombatSkillSystem>())
	{
		// Same note as above
	}
}

void ACombatCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the respawn timer
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Look);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Look);

		// Combo Attack
		EnhancedInputComponent->BindAction(ComboAttackAction, ETriggerEvent::Started, this, &ACombatCharacter::ComboAttackPressed);

		// Charged Attack
		EnhancedInputComponent->BindAction(ChargedAttackAction, ETriggerEvent::Started, this, &ACombatCharacter::ChargedAttackPressed);
		EnhancedInputComponent->BindAction(ChargedAttackAction, ETriggerEvent::Completed, this, &ACombatCharacter::ChargedAttackReleased);

		// Lock-On
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ACombatCharacter::LockOnPressed);
	}
}

void ACombatCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// update the respawn transform on the Player Controller
	if (ACombatPlayerController* PC = Cast<ACombatPlayerController>(GetController()))
	{
		PC->SetRespawnTransform(GetActorTransform());
	}
}

