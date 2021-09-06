// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class A : ModuleRules
{
    public A(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
            {"Core", "CoreUObject", "Engine", "InputCore", "Json", "JsonUtilities", "UMG"});
        // PrivateDependencyModuleNames.AddRange(new string[] {"Slate", "SlateCore"});
    }
}