// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;

public class DogFight : ModuleRules
{
	public DogFight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.Add("DogFight");
		PublicIncludePaths.Add("DogFight/Public");

		PrivateIncludePaths.Add("DogFight/Private");

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"HeadMountedDisplay",
				"NavigationSystem",
				"AIModule",
				"OnlineSubsystem",
				"OnlineSubsystemUtils",
				"Slate",
				"SlateCore",
				"GameplayTasks",
				"GameplayTags",
				"Niagara",
				"ImGui",
				"InGameScript",
				"UnLua",
				"ProjectFramework",
				"AttributeSystem",
				"DamageSystem",
				"LuaIntegration",
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CardSystem",
				"NetCore",
			}
		);
		
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
	}
}
