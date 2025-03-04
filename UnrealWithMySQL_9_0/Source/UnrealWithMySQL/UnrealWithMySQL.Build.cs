// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class UnrealWithMySQL : ModuleRules
{
    private string MySQL_Directory
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "..", "MySQL")); }
    }

    public UnrealWithMySQL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Include ��� �߰�
        PublicIncludePaths.AddRange(
            new string[] {
                Path.Combine(MySQL_Directory,"include"),
                Path.Combine(MySQL_Directory,"include/mysql"),
                Path.Combine(MySQL_Directory,"include/jdbc"),
                Path.Combine(MySQL_Directory,"include/jdbc/cppconn")
            }
        );

        // MySQL 9.0 ���̺귯�� ����
        PublicAdditionalLibraries.AddRange(
            new string[] {
                Path.Combine(MySQL_Directory, "lib64/vs14/mysqlcppconn.lib"),
                Path.Combine(MySQL_Directory, "lib64/vs14/mysqlcppconn-static.lib"),
                Path.Combine(MySQL_Directory, "lib64/vs14/libssl.lib"),        // OpenSSL Library
                Path.Combine(MySQL_Directory, "lib64/vs14/libcrypto.lib")      // OpenSSL Library
            }
        );

        // DLL ���� ��Ÿ�� ���Ӽ� ����
        RuntimeDependencies.Add(
            "$(BinaryOutputDir)/mysqlcppconn-10-vs14.dll",
            Path.Combine(MySQL_Directory, "lib64/mysqlcppconn-10-vs14.dll"));    // MySQL Connector�� �ֿ� ���� ���̺귯��.
        RuntimeDependencies.Add(
            "$(BinaryOutputDir)/mysqlcppconnx-2-vs14.dll",
            Path.Combine(MySQL_Directory, "lib64/mysqlcppconnx-2-vs14.dll"));    // Ȯ�� ����� �����ϴ� �߰� ���̺귯��.
        RuntimeDependencies.Add(
            "$(BinaryOutputDir)/libssl-3-x64.dll",
            Path.Combine(MySQL_Directory, "lib64/libssl-3-x64.dll"));            // OpenSSL ���� ���̺귯��.
        RuntimeDependencies.Add(
            "$(BinaryOutputDir)/libcrypto-3-x64.dll",
            Path.Combine(MySQL_Directory, "lib64/libcrypto-3-x64.dll"));         // OpenSSL ���� ���̺귯��.

        PublicDefinitions.Add("CONCPP_BUILD_SHARED=1");
    }
}
