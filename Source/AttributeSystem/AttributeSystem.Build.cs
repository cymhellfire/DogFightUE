using UnrealBuildTool;

public class AttributeSystem : ModuleRules
{
    public AttributeSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add("AttributeSystem/Public");
        PrivateIncludePaths.Add("AttributeSystem/Private");

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
                "NetCore",
            }
        );
    }
}