﻿using UnrealBuildTool;

public class CardSystem : ModuleRules
{
	public CardSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.Add("CardSystem/Public");
		PrivateIncludePaths.Add("CardSystem/Private");
		
		PublicDependencyModuleNames.AddRange(
			new []
			{
				"Core",
				"AttributeSystem", 
				"UnLua",
				"LuaIntegration",
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new []
			{
				"CoreUObject",
				"Engine",
				"ProjectFramework",
				"UMG",
				"SlateCore",
				"InputCore",
				"NetCore",
				"EnhancedInput",
			}
		);
	}
}
