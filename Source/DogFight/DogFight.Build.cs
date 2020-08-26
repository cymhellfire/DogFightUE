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
		
		PrivateIncludePaths.Add("DogFight");
		PrivateIncludePaths.Add("DogFight/Public");
		// Add all sub folder
		var SourceCodePath = Path.Combine(ModuleDirectory, "Public/");
		List<string> SubFolderList = new List<string>();
		GetSubFolders(SourceCodePath, ref SubFolderList);
		foreach (var Folder in SubFolderList)
		{
			Console.WriteLine("Add sub folder: " + Folder);
		}
		
		PrivateIncludePaths.AddRange(SubFolderList);
		
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
				"SlateCore"
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
