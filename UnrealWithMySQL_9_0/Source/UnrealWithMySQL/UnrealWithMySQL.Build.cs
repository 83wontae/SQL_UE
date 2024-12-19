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
                Path.Combine(MySQL_Directory,"include"),
                Path.Combine(MySQL_Directory,"include/mysqlx"),
                Path.Combine(MySQL_Directory,"include/mysqlx/common"),
                Path.Combine(MySQL_Directory,"include/mysqlx/devapi")
            }
        );

        // MySQL 9.0 라이브러리 연결
        PublicAdditionalLibraries.AddRange(
            new string[] {
                Path.Combine(MySQL_Directory, "lib64/vs14/mysqlcppconnx.lib"), // 주요 동적 라이브러리를 연결하기 위한 Import Library.( 이전 버전 mysqlcppconn8.lib )
                Path.Combine(MySQL_Directory, "lib64/vs14/libssl.lib"),        // OpenSSL Library
                Path.Combine(MySQL_Directory, "lib64/vs14/libcrypto.lib")      // OpenSSL Library
            }
        );

        // DLL 파일 런타임 종속성 설정
        RuntimeDependencies.Add(
            "$(BinaryOutputDir)/mysqlcppconn-10-vs14.dll", 
            Path.Combine(MySQL_Directory, "lib64/mysqlcppconn-10-vs14.dll"));    // MySQL Connector의 주요 동적 라이브러리.
        RuntimeDependencies.Add(
            "$(BinaryOutputDir)/mysqlcppconnx-2-vs14.dll", 
            Path.Combine(MySQL_Directory, "lib64/mysqlcppconnx-2-vs14.dll"));    // 확장 기능을 지원하는 추가 라이브러리.
        RuntimeDependencies.Add(
            "$(BinaryOutputDir)/libssl-3-x64.dll", 
            Path.Combine(MySQL_Directory, "lib64/libssl-3-x64.dll"));            // OpenSSL 동적 라이브러리.
        RuntimeDependencies.Add(
            "$(BinaryOutputDir)/libcrypto-3-x64.dll", 
            Path.Combine(MySQL_Directory, "lib64/libcrypto-3-x64.dll"));         // OpenSSL 동적 라이브러리.
    }
}
