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
				"CustomizableCard",
				"Slate",
				"SlateCore",
				"GameplayTasks"
			}
		);
		
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
	}

	private void GetSubFolders(string RootFolder, ref List<string> SubFolderList)
	{
		var SubFolders = Directory.GetDirectories(RootFolder);
		foreach (var SubFolder in SubFolders)
		{
			SubFolderList.Add(SubFolder);
			GetSubFolders(SubFolder, ref SubFolderList);
		}
	}
}
