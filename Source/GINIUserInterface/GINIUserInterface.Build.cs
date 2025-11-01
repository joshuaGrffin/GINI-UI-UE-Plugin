// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GINIUserInterface : ModuleRules
{
	public GINIUserInterface(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"CommonUI",
				"CommonInput",
				"DeveloperSettings",
				"UMG",
				"RenderCore",
				"GameplayTags",
				"EnhancedInput",
			}
		);
		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UMGEditor",
					"UnrealEd",
				});
		}
	}
}
