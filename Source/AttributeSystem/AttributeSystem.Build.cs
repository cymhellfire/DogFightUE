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

        bool bEnableDetailLog = true;
        if (Target.bBuildEditor && bEnableDetailLog)
        {
            PrivateDefinitions.Add("ATTR_DETAIL_LOG=1");
        }
        else
        {
            PrivateDefinitions.Add("ATTR_DETAIL_LOG=0");
        }
    }
}