using UnrealBuildTool;

public class ProjectFramework : ModuleRules
{
	public ProjectFramework(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.Add("ProjectFramework/Public");
		PrivateIncludePaths.Add("ProjectFramework/Private");
		
		PublicDependencyModuleNames.AddRange(
			new []
			{
				"Core",
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new []
			{
				"CoreUObject",
				"Engine",
				"OnlineSubsystem",
				"OnlineSubsystemUtils",
				"Slate",
				"UMG",
			}
		);
	}
}