// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "Variant_Combat/UI/CombatHUD.h"
#include "NinjaHUD.generated.h"

/**
 * HUD widget for ninja game - health bar positioned top-left
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API UNinjaHUD : public UCombatHUD
{
	GENERATED_BODY()

public:
	UNinjaHUD(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

protected:
	/** Health bar container (for positioning top-left) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Ninja|HUD")
	TObjectPtr<UWidget> HealthBarContainer;
};
