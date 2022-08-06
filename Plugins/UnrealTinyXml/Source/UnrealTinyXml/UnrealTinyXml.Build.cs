using System.IO;
using UnrealBuildTool;

public class UnrealTinyXml : ModuleRules
{
	public UnrealTinyXml(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = false;

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
