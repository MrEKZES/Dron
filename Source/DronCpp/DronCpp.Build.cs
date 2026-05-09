using UnrealBuildTool;

public class DronCpp : ModuleRules
{
	public DronCpp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"NavigationSystem",
			"AIModule",
			"GameplayTasks",
			"ImageWrapper",
			"Slate",
			"SlateCore"
		});
        
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}