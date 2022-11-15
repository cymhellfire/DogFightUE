using UnrealBuildTool;

public class DamageSystem : ModuleRules
{
    public DamageSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicIncludePaths.Add("DamageSystem/Public");
        PrivateIncludePaths.Add("DamageSystem/Private");

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "AttributeSystem",
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
            }
        );
    }
}