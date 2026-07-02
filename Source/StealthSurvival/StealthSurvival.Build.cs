// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StealthSurvival : ModuleRules
{
	public StealthSurvival(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"SmartObjectsModule",
			"GameplayTags",
			"GameplayAbilities",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"GameplayTasks",
			"StateTreeModule",
			"NavigationSystem",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime" });

		PublicIncludePaths.AddRange(new string[] {
			"StealthSurvival",
			"StealthSurvival/Player",
			"StealthSurvival/Animation",
			"StealthSurvival/GameMode",
			"StealthSurvival/AI",
			"StealthSurvival/Gameplay",
			"StealthSurvival/UI",
			"StealthSurvival/Interfaces"
		});
	}
}
