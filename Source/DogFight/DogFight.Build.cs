// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class DogFight : ModuleRules
{
	public DogFight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateIncludePaths.Add("DogFight");
		PrivateIncludePaths.Add("DogFight/Public");
		// Add all sub folder
		var SourceCodePath = Path.Combine(ModuleDirectory, "Public/");
		var SubFolders = Directory.GetDirectories(SourceCodePath);
		foreach (var Folder in SubFolders)
		{
			Console.WriteLine("Add sub folder: " + Folder);
		}
		
		PrivateIncludePaths.AddRange(SubFolders);
		
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
				"CustomizableCard"
			}
		);
		
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
	}
}
