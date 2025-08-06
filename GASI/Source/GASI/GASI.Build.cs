// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASI : ModuleRules
{
	public GASI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
            {
            "Core", 
			"InputCore", 
			"EnhancedInput", 
			"GameplayAbilities",
			"GameplayTasks", 
			"GameplayTags"
        });


        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "EnhancedInput"
				// ... add private dependencies that you statically link with here ...	
			}
			);
	}
}
