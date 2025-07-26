// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BlasterDream : ModuleRules
{
	public BlasterDream(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem", "AIModule", "Niagara", "MotionWarping", "Paper2D", "GameplayTasks", "MovieScene", "MovieSceneTracks", "LevelSequence","Slate", "SlateCore", "MediaAssets", "PhysicsCore", "Landscape", "CinematicCamera"});
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
