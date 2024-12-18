// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class UnrealWithMySQL : ModuleRules
{
    private string MySQL
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../MySQL/")); }
    }

    public UnrealWithMySQL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(
            new string[] {
                "UnrealWithMySQL",
                Path.Combine(MySQL,"include"), // 헤더 파일 경로 추가
                Path.Combine(MySQL,"include/mysqlx") // 헤더 파일 경로 추가
            }
        );

        // lib 파일 경로 추가
        PublicAdditionalLibraries.Add(Path.Combine(MySQL, "lib64/vs14/mysqlcppconn8.lib"));

        // DLL 파일 설정
        RuntimeDependencies.Add(Path.Combine(MySQL, "lib64/mysqlcppconn8-2-vs14.dll"));

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
