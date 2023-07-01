using UnrealBuildTool;

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
                "LuaIntegration",
                "ProjectFramework",
                "CardSystem",
                "UnLua",
                "DamageSystem",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "NetCore",
                "AIModule",
                "EnhancedInput",
                "Niagara",
                "LevelSequence",
                "MovieScene",
                "UMG",
                "Lua",
                "AttributeSystem",
                "ImGui",
                "CommonUser",
            }
        );
        
    }
}