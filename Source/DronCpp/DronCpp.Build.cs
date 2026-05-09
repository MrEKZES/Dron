using UnrealBuildTool;

public class DronCpp : ModuleRules
{
	public DronCpp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		if (Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			OptimizeCode = CodeOptimization.InShippingBuildsOnly;
		}

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
		
		if (Target.bBuildEditor)
		{
			MinFilesUsingPrecompiledHeaderOverride = 1;
			bUseUnity = false;
		}
	}
}