// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Variant_Combat/CombatCharacter.h"
#include "NinjaCharacter.generated.h"

class UInputAction;
class UAnimMontage;

/** Type of acrobatic flip (drives mesh rotation axis and total angle) */
UENUM(BlueprintType)
enum class ENinjaFlipType : uint8
{
	None,
	Backflip   UMETA(DisplayName = "Backflip"),
	Frontflip  UMETA(DisplayName = "Frontflip"),
	SideflipLeft  UMETA(DisplayName = "Sideflip Left"),
	SideflipRight UMETA(DisplayName = "Sideflip Right"),
	Flip360    UMETA(DisplayName = "360 Flip")
};

/**
 * OoT-style ninja + acrobatics: double jump, backflip, frontflip, sideflip, 360 flip.
 * Punch/kick (combo + charged + optional kick) work during flips — combine motion freely.
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API ANinjaCharacter : public ACombatCharacter
{
	GENERATED_BODY()

public:
	ANinjaCharacter();

	// ---- Roll (unchanged) ----
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> RollAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ninja|Roll", meta = (ClampMin = 0.1f, ClampMax = 1.0f, Units = "s"))
	float RollDuration = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ninja|Roll", meta = (ClampMin = 0, Units = "cm/s"))
	float RollSpeed = 800.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ninja|Roll")
	bool bIsRolling = false;
	UFUNCTION(BlueprintCallable, Category = "Ninja")
	void DoRoll();

	// ---- Double jump ----
	/** Number of jumps allowed (1 = single, 2 = double jump). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ninja|Acrobatics", meta = (ClampMin = 1, ClampMax = 3))
	int32 JumpCountMax = 2;

	// ---- Flips ----
	UPROPERTY(EditAnywhere, Category = "Input|Flips")
	TObjectPtr<UInputAction> BackflipAction;
	UPROPERTY(EditAnywhere, Category = "Input|Flips")
	TObjectPtr<UInputAction> FrontflipAction;
	UPROPERTY(EditAnywhere, Category = "Input|Flips")
	TObjectPtr<UInputAction> SideflipLeftAction;
	UPROPERTY(EditAnywhere, Category = "Input|Flips")
	TObjectPtr<UInputAction> SideflipRightAction;
	UPROPERTY(EditAnywhere, Category = "Input|Flips")
	TObjectPtr<UInputAction> Flip360Action;

	/** Flip duration in seconds (mesh rotation + arc). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ninja|Acrobatics", meta = (ClampMin = 0.2f, ClampMax = 1.5f, Units = "s"))
	float FlipDuration = 0.5f;
	/** Upward impulse when starting a flip (adds to any existing velocity). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ninja|Acrobatics", meta = (ClampMin = 0, Units = "cm/s"))
	float FlipUpImpulse = 350.0f;
	/** Horizontal impulse scale (forward/back/side) when starting a flip. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ninja|Acrobatics", meta = (ClampMin = 0, Units = "cm/s"))
	float FlipHorizontalImpulse = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ninja|Acrobatics")
	bool bIsFlipping = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ninja|Acrobatics")
	ENinjaFlipType CurrentFlipType = ENinjaFlipType::None;

	UFUNCTION(BlueprintCallable, Category = "Ninja")
	void DoBackflip();
	UFUNCTION(BlueprintCallable, Category = "Ninja")
	void DoFrontflip();
	UFUNCTION(BlueprintCallable, Category = "Ninja")
	void DoSideflipLeft();
	UFUNCTION(BlueprintCallable, Category = "Ninja")
	void DoSideflipRight();
	UFUNCTION(BlueprintCallable, Category = "Ninja")
	void DoFlip360();

	// ---- Kick (optional; combine with flips) ----
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> KickAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ninja|Combat")
	TObjectPtr<UAnimMontage> KickMontage;
	UFUNCTION(BlueprintCallable, Category = "Ninja")
	void DoKick();

	/** Returns true if the last damage we dealt was during a flip (for style scoring); clears the flag. */
	UFUNCTION(BlueprintCallable, Category = "Ninja")
	bool ConsumeLastHitWasDuringFlip();

	virtual void DoAttackTrace(FName DamageSourceBone) override;

protected:
	/** Set when we deal damage while flipping; read by ConsumeLastHitWasDuringFlip for scoring. */
	bool bLastDealtDamageWhileFlipping = false;
	FVector RollDirection;
	float RollTimeRemaining = 0.0f;

	/** Mesh rotation at flip start (so we can add flip delta and reset after). */
	FRotator MeshFlipBaseRotation;
	float FlipElapsed = 0.0f;
	/** Total degrees to rotate for current flip (180 or 360). */
	float FlipTotalDegrees = 0.0f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Start a flip (impulse + state). Can be called from ground or air. */
	void StartFlip(ENinjaFlipType FlipType, const FVector& HorizontalDir);
	/** Update mesh rotation for current flip; call when bIsFlipping. */
	void UpdateFlipMeshRotation(float DeltaTime);
	/** End flip and reset mesh. */
	void EndFlip();
};
