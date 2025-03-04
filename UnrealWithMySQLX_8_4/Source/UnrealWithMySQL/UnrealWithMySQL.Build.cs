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
                Path.Combine(MySQL_Directory,"include"), // 헤더 파일 경로 추가
                Path.Combine(MySQL_Directory,"include/mysqlx"), // 헤더 파일 경로 추가
                Path.Combine(MySQL_Directory,"include/mysqlx/common"), // 헤더 파일 경로 추가
                Path.Combine(MySQL_Directory,"include/mysqlx/devapi") // 헤더 파일 경로 추가
            }
        );

        // MySQL 라이브러리 연결
        PublicAdditionalLibraries.AddRange(
            new string[] {
                Path.Combine(MySQL_Directory, "lib64/vs14/mysqlcppconn8.lib"),
                Path.Combine(MySQL_Directory, "lib64/vs14/libssl.lib"),
                Path.Combine(MySQL_Directory, "lib64/vs14/libcrypto.lib")
            }
        );

        // DLL 파일 런타임 종속성 설정
        RuntimeDependencies.Add("$(BinaryOutputDir)/mysqlcppconn8-2-vs14.dll", Path.Combine(MySQL_Directory, "lib64/mysqlcppconn8-2-vs14.dll"));
        RuntimeDependencies.Add("$(BinaryOutputDir)/libssl-3-x64.dll", Path.Combine(MySQL_Directory, "lib64/libssl-3-x64.dll"));
        RuntimeDependencies.Add("$(BinaryOutputDir)/libcrypto-3-x64.dll", Path.Combine(MySQL_Directory, "lib64/libcrypto-3-x64.dll"));
    }
}
