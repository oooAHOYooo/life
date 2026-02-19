// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CPPo1 : ModuleRules
{
	public CPPo1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"CPPo1",
			"CPPo1/Variant_Platforming",
			"CPPo1/Variant_Platforming/Animation",
			"CPPo1/Variant_Combat",
			"CPPo1/Variant_Combat/AI",
			"CPPo1/Variant_Combat/Animation",
			"CPPo1/Variant_Combat/Gameplay",
			"CPPo1/Variant_Combat/Interfaces",
			"CPPo1/Variant_Combat/UI",
			"CPPo1/Variant_SideScrolling",
			"CPPo1/Variant_SideScrolling/AI",
			"CPPo1/Variant_SideScrolling/Gameplay",
			"CPPo1/Variant_SideScrolling/Interfaces",
			"CPPo1/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
