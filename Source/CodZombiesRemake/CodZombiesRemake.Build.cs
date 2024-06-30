// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CodZombiesRemake : ModuleRules
{
	public CodZombiesRemake(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "HeadMountedDisplay", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
