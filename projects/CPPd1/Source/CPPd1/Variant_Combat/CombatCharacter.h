// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatAttacker.h"
#include "CombatDamageable.h"
#include "Animation/AnimInstance.h"
#include "CombatCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UCombatLifeBar;
class UWidgetComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogCombatCharacter, Log, All);

/**
 *  An enhanced Third Person Character with melee combat capabilities:
 *  - Combo attack string
 *  - Press and hold charged attack
 *  - Damage dealing and reaction
 *  - Death
 *  - Respawning
 */
UCLASS(BlueprintType, Blueprintable)
class ACombatCharacter : public ACharacter, public ICombatAttacker, public ICombatDamageable
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Life bar widget component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* LifeBar;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Combo Attack Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ComboAttackAction;

	/** Charged Attack Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ChargedAttackAction;

	/** Lock-On Input Action (toggle or hold) */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* LockOnAction;

	/** Lock-on: search radius for targets */
	UPROPERTY(EditAnywhere, Category ="Lock-On", meta = (ClampMin = 0, Units = "cm"))
	float LockOnRadius = 1500.0f;

	/** Lock-on: rotation blend speed toward target */
	UPROPERTY(EditAnywhere, Category ="Lock-On", meta = (ClampMin = 0, Units = "deg/s"))
	float LockOnRotationSpeed = 360.0f;

	/** Current lock-on target (enemy or other actor with CPPd1LockOnTargetComponent) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Lock-On")
	TObjectPtr<AActor> LockOnTarget;

	/** Ghost character that mirrors this one when playing solo (this drives it with the same input). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="CPPd1|Ghost")
	TObjectPtr<ACombatCharacter> GhostCharacter;

public:

	/** When true, this is a ghost copy (solo 2P); takes no damage and mirrors the main character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="CPPd1|Ghost")
	bool bIsGhost = false;

	/** Control rotation used when not possessed (e.g. ghost mirroring main). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="CPPd1|Ghost")
	FRotator StoredControlRotation;

protected:

	/** Max amount of HP the character will have on respawn */
	UPROPERTY(EditAnywhere, Category="Damage", meta = (ClampMin = 0, ClampMax = 100))
	float MaxHP = 5.0f;

	/** Current amount of HP the character has */
	UPROPERTY(VisibleAnywhere, Category="Damage")
	float CurrentHP = 0.0f;

	/** Life bar widget fill color */
	UPROPERTY(EditAnywhere, Category="Damage")
	FLinearColor LifeBarColor;

	/** Name of the pelvis bone, for damage ragdoll physics */
	UPROPERTY(EditAnywhere, Category="Damage")
	FName PelvisBoneName;

	/** Pointer to the life bar widget */
	UPROPERTY(EditAnywhere, Category="Damage")
	TObjectPtr<UCombatLifeBar> LifeBarWidget;

	/** Max amount of time that may elapse for a non-combo attack input to not be considered stale */
	UPROPERTY(EditAnywhere, Category="Melee Attack", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float AttackInputCacheTimeTolerance = 1.0f;

	/** Time at which an attack button was last pressed */
	float CachedAttackInputTime = 0.0f;

	/** If true, the character is currently playing an attack animation */
	bool bIsAttacking = false;

	/** Distance ahead of the character that melee attack sphere collision traces will extend */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Trace", meta = (ClampMin = 0, ClampMax = 500, Units="cm"))
	float MeleeTraceDistance = 75.0f;

	/** Radius of the sphere trace for melee attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Trace", meta = (ClampMin = 0, ClampMax = 200, Units = "cm"))
	float MeleeTraceRadius = 75.0f;

	/** Amount of damage a melee attack will deal */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 100))
	float MeleeDamage = 1.0f;

	/** Amount of knockback impulse a melee attack will apply */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm/s"))
	float MeleeKnockbackImpulse = 250.0f;

	/** Amount of upwards impulse a melee attack will apply */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm/s"))
	float MeleeLaunchImpulse = 300.0f;

	/** AnimMontage that will play for combo attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Combo")
	UAnimMontage* ComboAttackMontage;

	/** Names of the AnimMontage sections that correspond to each stage of the combo attack */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Combo")
	TArray<FName> ComboSectionNames;

	/** Max amount of time that may elapse for a combo attack input to not be considered stale */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Combo", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float ComboInputCacheTimeTolerance = 0.45f;

	/** Index of the current stage of the melee attack combo */
	int32 ComboCount = 0;

	/** AnimMontage that will play for charged attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged")
	UAnimMontage* ChargedAttackMontage;

	/** Name of the AnimMontage section that corresponds to the charge loop */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged")
	FName ChargeLoopSection;

	/** Name of the AnimMontage section that corresponds to the attack */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged")
	FName ChargeAttackSection;

	/** Flag that determines if the player is currently holding the charged attack input */
	bool bIsChargingAttack = false;
	
	/** If true, the charged attack hold check has been tested at least once */
	bool bHasLoopedChargedAttack = false;

	/** Camera boom length while the character is dead */
	UPROPERTY(EditAnywhere, Category="Camera", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float DeathCameraDistance = 400.0f;

	/** Camera boom length when the character respawns */
	UPROPERTY(EditAnywhere, Category="Camera", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float DefaultCameraDistance = 100.0f;

	/** Time to wait before respawning the character */
	UPROPERTY(EditAnywhere, Category="Respawn", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RespawnTime = 3.0f;

	// ========== TUNING VARIABLES - EXPOSED FOR INSPECTOR ==========

	/** Character movement speed (walk speed) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Movement", meta = (ClampMin = 0, Units = "cm/s"))
	float MovementSpeed = 400.0f;

	/** Character jump velocity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Movement", meta = (ClampMin = 0, Units = "cm/s"))
	float JumpVelocity = 500.0f;

	/** Air control amount (0.0 = no control, 1.0 = full control) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Movement", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float AirControl = 0.35f;

	/** Character rotation rate (how fast character turns) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Movement", meta = (ClampMin = 0, Units = "deg/s"))
	float RotationRate = 500.0f;

	// Trick System Tuning
	/** Maximum time between trick inputs to maintain combo (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Trick System", meta = (ClampMin = 0.0f, ClampMax = 2.0f, Units = "s"))
	float TrickInputGapTime = 0.5f;

	/** Base flow points awarded for any trick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Trick System", meta = (ClampMin = 0.0f))
	float BaseTrickFlowPoints = 10.0f;

	/** Flow point multiplier for complex tricks */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Trick System", meta = (ClampMin = 1.0f))
	float ComplexTrickMultiplier = 1.5f;

	// Skill System Tuning
	/** Starting skill level for all skills (0-100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Skill System", meta = (ClampMin = 0.0f, ClampMax = 100.0f))
	float StartingSkillLevel = 0.0f;

	/** Experience multiplier for skill gains (1.0 = normal, 2.0 = double) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Skill System", meta = (ClampMin = 0.1f, ClampMax = 10.0f))
	float SkillExperienceMultiplier = 1.0f;

	/** Base damage multiplier at skill level 0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Skill System", meta = (ClampMin = 0.1f))
	float BaseDamageMultiplier = 0.5f;

	/** Maximum damage multiplier at skill level 100 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Skill System", meta = (ClampMin = 1.0f))
	float MaxDamageMultiplier = 2.0f;

	// Stamina System Tuning
	/** Maximum stamina (0.0 to 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Stamina", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float MaxStamina = 1.0f;

	/** Stamina drain per light attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Stamina", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float LightAttackStaminaCost = 0.10f;

	/** Stamina drain per heavy attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Stamina", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float HeavyAttackStaminaCost = 0.25f;

	/** Stamina drain per second while blocking */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Stamina", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float BlockStaminaDrainRate = 0.15f;

	/** Base stamina recovery rate per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Stamina", meta = (ClampMin = 0.0f))
	float BaseStaminaRecoveryRate = 0.2f;

	/** Time delay before stamina starts recovering after drain */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Stamina", meta = (ClampMin = 0.0f, Units = "s"))
	float StaminaRecoveryDelay = 1.0f;

	/** Stamina threshold for exhaustion (below this = exhausted) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Stamina", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float ExhaustionThreshold = 0.2f;

	// Flow System Tuning
	/** Maximum flow meter (0.0 to 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Flow System", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float MaxFlow = 1.0f;

	/** Flow decay rate per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Flow System", meta = (ClampMin = 0.0f))
	float FlowDecayRate = 0.1f;

	/** Flow threshold to enter flow state (0.0 to 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Flow System", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float FlowStateThreshold = 0.7f;

	/** Damage/speed multiplier when in flow state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Flow System", meta = (ClampMin = 1.0f))
	float FlowStateMultiplier = 1.5f;

	/** Flow points per combo hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Flow System", meta = (ClampMin = 0.0f))
	float FlowPointsPerCombo = 5.0f;

	/** Time before combo resets (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Flow System", meta = (ClampMin = 0.0f, Units = "s"))
	float ComboResetTime = 3.0f;

	// Advanced Mechanics Tuning
	/** Perfect parry window duration (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Advanced Mechanics", meta = (ClampMin = 0.0f, ClampMax = 1.0f, Units = "s"))
	float PerfectParryWindow = 0.1f;

	/** Good parry window duration (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Advanced Mechanics", meta = (ClampMin = 0.0f, ClampMax = 1.0f, Units = "s"))
	float GoodParryWindow = 0.2f;

	/** Perfect parry damage multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Advanced Mechanics", meta = (ClampMin = 1.0f))
	float PerfectParryDamageMultiplier = 2.0f;

	/** Counter-attack window after parry (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Advanced Mechanics", meta = (ClampMin = 0.0f, Units = "s"))
	float CounterAttackWindow = 1.0f;

	/** Base block effectiveness (damage reduction, 0.0 to 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Advanced Mechanics", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float BaseBlockEffectiveness = 0.5f;

	// General Combat Tuning
	/** Global damage multiplier (affects all damage dealt) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Combat", meta = (ClampMin = 0.0f))
	float GlobalDamageMultiplier = 1.0f;

	/** Global defense multiplier (affects all damage received) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Combat", meta = (ClampMin = 0.0f))
	float GlobalDefenseMultiplier = 1.0f;

	/** Invincibility frames after taking damage (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tuning|Combat", meta = (ClampMin = 0.0f, Units = "s"))
	float InvincibilityFrames = 0.5f;

	/** Time since last damage taken */
	float TimeSinceLastDamage = 0.0f;

	/** Attack montage ended delegate */
	FOnMontageEnded OnAttackMontageEnded;

	/** Character respawn timer */
	FTimerHandle RespawnTimer;

	/** Copy of the mesh's transform so we can reset it after ragdoll animations */
	FTransform MeshStartingTransform;

public:
	
	/** Constructor */
	ACombatCharacter();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for combo attack input */
	void ComboAttackPressed();

	/** Called for combo attack input pressed */
	void ChargedAttackPressed();

	/** Called for combo attack input released */
	void ChargedAttackReleased();

	/** Called when lock-on input is triggered */
	void LockOnPressed();

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles combo attack pressed from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoComboAttackStart();

	/** Handles combo attack released from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoComboAttackEnd();

	/** Handles charged attack pressed from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoChargedAttackStart();

	/** Handles charged attack released from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoChargedAttackEnd();

	/** Lock on to nearest valid target in radius. */
	UFUNCTION(BlueprintCallable, Category="Lock-On")
	virtual void DoLockOn();

	/** Clear current lock-on target. */
	UFUNCTION(BlueprintCallable, Category="Lock-On")
	virtual void DoClearLockOn();

	/** Cycle to next lock-on target in radius (or clear if none). */
	UFUNCTION(BlueprintCallable, Category="Lock-On")
	virtual void DoCycleLockOn();

	/** Set the ghost character that mirrors this one (solo play). Ghost receives the same input. */
	UFUNCTION(BlueprintCallable, Category="CPPd1|Ghost")
	virtual void SetGhostCharacter(ACombatCharacter* Ghost);

	/** Get the ghost character (if any). */
	UFUNCTION(BlueprintPure, Category="CPPd1|Ghost")
	ACombatCharacter* GetGhostCharacter() const { return GhostCharacter; }

	/** Get control rotation (from controller or StoredControlRotation when unpossessed). */
	UFUNCTION(BlueprintPure, Category="CPPd1|Ghost")
	FRotator GetEffectiveControlRotation() const;

protected:

	/** Resets the character's current HP to maximum */
	void ResetHP();

	/** Performs a combo attack */
	void ComboAttack();

	/** Performs a charged attack */
	void ChargedAttack();

	/** Called from a delegate when the attack montage ends */
	void AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	
public:

	// ~begin CombatAttacker interface

	/** Performs the collision check for an attack */
	virtual void DoAttackTrace(FName DamageSourceBone) override;

	/** Performs the combo string check */
	virtual void CheckCombo() override;

	/** Performs the charged attack hold check */
	virtual void CheckChargedAttack() override;

	// ~end CombatAttacker interface

	// ~begin CombatDamageable interface

	/** Handles damage and knockback events */
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse) override;

	/** Handles death events */
	virtual void HandleDeath() override;

	/** Handles healing events */
	virtual void ApplyHealing(float Healing, AActor* Healer) override;

	// ~end CombatDamageable interface

	/** Called from the respawn timer to destroy and re-create the character */
	void RespawnCharacter();

public:

	/** Overrides the default TakeDamage functionality */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Overrides landing to reset damage ragdoll physics */
	virtual void Landed(const FHitResult& Hit) override;

protected:

	/** Blueprint handler to play damage dealt effects */
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void DealtDamage(float Damage, const FVector& ImpactPoint);

	/** Blueprint handler to play damage received effects */
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void ReceivedDamage(float Damage, const FVector& ImpactPoint, const FVector& DamageDirection);

protected:

	/** Initialization */
	virtual void BeginPlay() override;

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Initialize tuning variables for component systems */
	void InitializeTuningVariables();

	/** Cleanup */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Handles input bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Handles possessed initialization */
	virtual void NotifyControllerChanged() override;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
