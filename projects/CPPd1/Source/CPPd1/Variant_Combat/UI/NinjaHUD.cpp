// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Combat/UI/NinjaHUD.h"
#include "Components/Widget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Slate/SlateBrushAsset.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/Engine.h"

UNinjaHUD::UNinjaHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UNinjaHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// Build the HUD layout dynamically if widgets aren't already set up
	if (!HealthBar)
	{
		BuildDynamicLayout();
	}
}

void UNinjaHUD::BuildDynamicLayout()
{
	// Get root widget (should be a Canvas Panel)
	UWidget* RootWidget = GetRootWidget();
	UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(RootWidget);
	
	// If no root widget, create one
	if (!CanvasPanel)
	{
		CanvasPanel = NewObject<UCanvasPanel>(this);
		WidgetTree->RootWidget = CanvasPanel;
	}

	// Create health bar container (top-left)
	UHorizontalBox* HealthContainer = NewObject<UHorizontalBox>(this);
	HealthBarContainer = HealthContainer;

	// Create health label
	UTextBlock* HealthLabel = NewObject<UTextBlock>(this);
	HealthLabel->SetText(FText::FromString(TEXT("Health:")));
	HealthLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	
	// Create health bar
	UProgressBar* NewHealthBar = NewObject<UProgressBar>(this);
	NewHealthBar->SetPercent(1.0f);
	NewHealthBar->SetFillColorAndOpacity(FLinearColor::Red);
	
	// Add to horizontal box
	UPanelSlot* LabelSlot = HealthContainer->AddChild(HealthLabel);
	UPanelSlot* BarSlot = HealthContainer->AddChild(NewHealthBar);
	
	// Set health bar size
	if (UHorizontalBoxSlot* BarBoxSlot = Cast<UHorizontalBoxSlot>(BarSlot))
	{
		FSlateChildSize SizeRule;
		SizeRule.SizeRule = ESlateSizeRule::Fill;
		SizeRule.Value = 1.0f;
		BarBoxSlot->SetSize(SizeRule);
		BarBoxSlot->SetPadding(FMargin(10.0f, 0.0f, 0.0f, 0.0f));
	}

	// Add container to canvas and position top-left
	UPanelSlot* ContainerSlot = CanvasPanel->AddChild(HealthContainer);
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ContainerSlot))
	{
		CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f)); // Top-left anchor
		CanvasSlot->SetOffsets(FMargin(20.0f, 20.0f, 0.0f, 0.0f)); // 20px from top-left
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f)); // Top-left alignment
	}

	// Store reference to health bar
	HealthBar = NewHealthBar;

	// Create stamina bar if needed (optional, positioned below health)
	if (!StaminaBar)
	{
		UHorizontalBox* StaminaContainer = NewObject<UHorizontalBox>(this);
		UTextBlock* StaminaLabel = NewObject<UTextBlock>(this);
		StaminaLabel->SetText(FText::FromString(TEXT("Stamina:")));
		StaminaLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		
		UProgressBar* NewStaminaBar = NewObject<UProgressBar>(this);
		NewStaminaBar->SetPercent(1.0f);
		NewStaminaBar->SetFillColorAndOpacity(FLinearColor::Blue);
		
		StaminaContainer->AddChild(StaminaLabel);
		UPanelSlot* StaminaBarSlot = StaminaContainer->AddChild(NewStaminaBar);
		if (UHorizontalBoxSlot* StaminaBoxSlot = Cast<UHorizontalBoxSlot>(StaminaBarSlot))
		{
			FSlateChildSize SizeRule;
			SizeRule.SizeRule = ESlateSizeRule::Fill;
			SizeRule.Value = 1.0f;
			StaminaBoxSlot->SetSize(SizeRule);
			StaminaBoxSlot->SetPadding(FMargin(10.0f, 0.0f, 0.0f, 0.0f));
		}

		UPanelSlot* StaminaContainerSlot = CanvasPanel->AddChild(StaminaContainer);
		if (UCanvasPanelSlot* StaminaCanvasSlot = Cast<UCanvasPanelSlot>(StaminaContainerSlot))
		{
			StaminaCanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f));
			StaminaCanvasSlot->SetOffsets(FMargin(20.0f, 60.0f, 0.0f, 0.0f)); // Below health bar
			StaminaCanvasSlot->SetAutoSize(true);
			StaminaCanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f));
		}

		StaminaBar = NewStaminaBar;
	}
}
