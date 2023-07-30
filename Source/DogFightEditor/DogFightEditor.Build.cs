using UnrealBuildTool;

public class DogFightEditor : ModuleRules
{
    public DogFightEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add("DogFightEditor/Public");
        PrivateIncludePaths.Add("DogFightEditor/Private");

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "UnrealEd",
                "DeveloperSettings",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DogFight",
                "ToolMenus", 
                "MainFrame",
                "InputCore",
                "EditorStyle",
                "DogFightGame",
            }
        );
    }
}