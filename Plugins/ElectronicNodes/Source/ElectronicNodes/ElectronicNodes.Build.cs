/* Copyright (C) 2021 Hugo ATTAL - All Rights Reserved
* This plugin is downloadable from the UE4 Marketplace
*/

using System.IO;
using UnrealBuildTool;

public class ElectronicNodes : ModuleRules
{
	public ElectronicNodes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		string enginePath = Path.GetFullPath(Target.RelativeEnginePath);

		PublicIncludePaths.AddRange(
			new string[] { }
		);

		PrivateIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(enginePath, "Source/Editor/AnimationBlueprintEditor/Private/")
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"InputCore",
				"Projects",
				"UnrealEd",
				"GraphEditor",
				"BlueprintGraph",
				"AnimGraph",
				"AnimationBlueprintEditor",
				"RigVM",
				"RigVMDeveloper",
				"AIGraph",
				"BehaviorTreeEditor",
				"DeveloperSettings",
				"EditorStyle",
				"WebBrowser"
			}
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
				{ }
		);
	}
}