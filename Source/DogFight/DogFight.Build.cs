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
		// Add all sub folder
		var SourceCodePath = Path.Combine(ModuleDirectory, "Public/");
		List<string> SubFolderList = new List<string>();
		GetSubFolders(SourceCodePath, ref SubFolderList);
		foreach (var Folder in SubFolderList)
		{
			Console.WriteLine("Add public sub folder: " + Folder);
		}
		PublicIncludePaths.AddRange(SubFolderList);
		
		PrivateIncludePaths.Add("DogFight/Private");
		// Add all sub folder
		SourceCodePath = Path.Combine(ModuleDirectory, "Private/");
		SubFolderList.Clear();
		GetSubFolders(SourceCodePath, ref SubFolderList);
		foreach (var Folder in SubFolderList)
		{
			Console.WriteLine("Add private sub folder: " + Folder);
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
