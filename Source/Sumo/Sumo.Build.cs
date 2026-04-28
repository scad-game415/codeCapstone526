// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sumo : ModuleRules
{
	public Sumo(ReadOnlyTargetRules Target) : base(Target)
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
			"Sumo",
			"Sumo/Variant_Platforming",
			"Sumo/Variant_Platforming/Animation",
			"Sumo/Variant_Combat",
			"Sumo/Variant_Combat/AI",
			"Sumo/Variant_Combat/Animation",
			"Sumo/Variant_Combat/Gameplay",
			"Sumo/Variant_Combat/Interfaces",
			"Sumo/Variant_Combat/UI",
			"Sumo/Variant_SideScrolling",
			"Sumo/Variant_SideScrolling/AI",
			"Sumo/Variant_SideScrolling/Gameplay",
			"Sumo/Variant_SideScrolling/Interfaces",
			"Sumo/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
