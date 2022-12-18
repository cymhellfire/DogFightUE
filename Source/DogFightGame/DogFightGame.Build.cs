﻿using UnrealBuildTool;

public class DogFightGame : ModuleRules
{
    public DogFightGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "GameFlow",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
            }
        );
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "ProjectFramework",
                "LuaIntegration",
            });
    }
}