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
                Path.Combine(MySQL,"include"), // ��� ���� ��� �߰�
                Path.Combine(MySQL,"include/mysqlx") // ��� ���� ��� �߰�
            }
        );

        // lib ���� ��� �߰�
        PublicAdditionalLibraries.Add(Path.Combine(MySQL, "lib64/vs14/mysqlcppconn8.lib"));

        // DLL ���� ����
        RuntimeDependencies.Add(Path.Combine(MySQL, "lib64/mysqlcppconn8-2-vs14.dll"));

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
