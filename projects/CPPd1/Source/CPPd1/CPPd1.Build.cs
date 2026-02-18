// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CPPd1 : ModuleRules
{
	public CPPd1(ReadOnlyTargetRules Target) : base(Target)
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
			"CPPd1",
			"CPPd1/Variant_Platforming",
			"CPPd1/Variant_Platforming/Animation",
			"CPPd1/Variant_Combat",
			"CPPd1/Variant_Combat/AI",
			"CPPd1/Variant_Combat/Animation",
			"CPPd1/Variant_Combat/Gameplay",
			"CPPd1/Variant_Combat/Interfaces",
			"CPPd1/Variant_Combat/UI",
			"CPPd1/Variant_SideScrolling",
			"CPPd1/Variant_SideScrolling/AI",
			"CPPd1/Variant_SideScrolling/Gameplay",
			"CPPd1/Variant_SideScrolling/Interfaces",
			"CPPd1/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
