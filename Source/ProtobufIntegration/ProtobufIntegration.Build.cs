using System;
using System.IO;
using EpicGames.Core;
using UnrealBuildTool;

public class ProtobufIntegration : ModuleRules
{
    private string ThirdPartyPath
    {
        get { return Path.Combine(ModuleDirectory, "ThirdParty/"); }
    }

    public ProtobufIntegration(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "ProjectFramework",
                "libprotobuf",
                "LuaIntegration",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
            }
        );

        var ProtobufLibPath = GetModuleDirectory("libprotobuf");
        var IncludePath = Path.Combine(ProtobufLibPath, "include");

        PublicSystemIncludePaths.AddRange(
            new string[]
            {
                IncludePath
            });

        // PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS");
        //
        // ShadowVariableWarningLevel = WarningLevel.Off;
        // bEnableUndefinedIdentifierWarnings = false;
        // bEnableExceptions = true;
    }
}