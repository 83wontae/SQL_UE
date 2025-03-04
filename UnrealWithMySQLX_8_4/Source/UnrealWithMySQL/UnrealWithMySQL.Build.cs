// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class UnrealWithMySQL : ModuleRules
{
    private string MySQL_Directory
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../MySQL/")); }
    }

    public UnrealWithMySQL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.AddRange(
            new string[] {
                Path.Combine(MySQL_Directory,"include"), // ��� ���� ��� �߰�
                Path.Combine(MySQL_Directory,"include/mysqlx"), // ��� ���� ��� �߰�
                Path.Combine(MySQL_Directory,"include/mysqlx/common"), // ��� ���� ��� �߰�
                Path.Combine(MySQL_Directory,"include/mysqlx/devapi") // ��� ���� ��� �߰�
            }
        );

        // MySQL ���̺귯�� ����
        PublicAdditionalLibraries.AddRange(
            new string[] {
                Path.Combine(MySQL_Directory, "lib64/vs14/mysqlcppconn8.lib"),
                Path.Combine(MySQL_Directory, "lib64/vs14/libssl.lib"),
                Path.Combine(MySQL_Directory, "lib64/vs14/libcrypto.lib")
            }
        );

        // DLL ���� ��Ÿ�� ���Ӽ� ����
        RuntimeDependencies.Add("$(BinaryOutputDir)/mysqlcppconn8-2-vs14.dll", Path.Combine(MySQL_Directory, "lib64/mysqlcppconn8-2-vs14.dll"));
        RuntimeDependencies.Add("$(BinaryOutputDir)/libssl-3-x64.dll", Path.Combine(MySQL_Directory, "lib64/libssl-3-x64.dll"));
        RuntimeDependencies.Add("$(BinaryOutputDir)/libcrypto-3-x64.dll", Path.Combine(MySQL_Directory, "lib64/libcrypto-3-x64.dll"));
    }
}
