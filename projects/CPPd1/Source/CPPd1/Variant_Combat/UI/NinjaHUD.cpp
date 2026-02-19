// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Combat/UI/NinjaHUD.h"
#include "Components/Widget.h"

UNinjaHUD::UNinjaHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UNinjaHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// Position health bar top-left in Blueprint
	// This is mainly for Blueprint designers - the actual positioning
	// should be done in the Blueprint widget designer
}
