using UnrealBuildTool;

public class ActionGameWeaponEditor : ModuleRules
{
    public ActionGameWeaponEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "ActionGameWeapon",
                "UnrealEd",
            }
        );
    }
}