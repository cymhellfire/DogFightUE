// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;

public class CustomizableCard : ModuleRules
{
	public CustomizableCard(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.Add("CustomizableCard/Public");

		PrivateIncludePaths.Add("CustomizableCard/Private");

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"HeadMountedDisplay",
				"NavigationSystem",
				"AIModule",
			}
		);
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
