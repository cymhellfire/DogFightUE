using System.IO;
using UnrealBuildTool;

public class UnrealTinyXml : ModuleRules
{
	public UnrealTinyXml(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
#if UE_5_2_OR_LATER
        IWYUSupport = IWYUSupport.None;
#else
        bEnforceIWYU = false;
#endif

		bEnableUndefinedIdentifierWarnings = false;
		ShadowVariableWarningLevel = WarningLevel.Off;
		
		PublicIncludePaths.AddRange(
			new []
			{
				Path.Combine(ModuleDirectory, "Public"),
			});
		
		PrivateIncludePaths.AddRange(
			new []
			{
				"UnrealTinyXml/Private",
			});
		
		PublicDependencyModuleNames.AddRange(
			new []
			{
				"Core",
			});
	}
}
