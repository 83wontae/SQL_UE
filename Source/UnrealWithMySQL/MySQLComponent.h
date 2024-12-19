// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <mysqlx/xdevapi.h> // MySQLX 헤더 파일
#include "MySQLComponent.generated.h"

// 블루프린트에 연결 성공 여부와 메시지를 반환하기 위한 Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMySQLConnectionResult, bool, bSuccess, const FString&, Message);

UCLASS(  Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALWITHMYSQL_API UMySQLComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMySQLComponent();
	~UMySQLComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 블루프린트에서 호출 가능한 MySQL 연결 함수
	UFUNCTION(BlueprintCallable, Category = "MySQL")
	void ConnectToDatabaseAsync(const FString& Host, int32 Port, const FString& Username, const FString& Password, const FString& Schema);

	// 연결 결과 이벤트 (블루프린트에서 바인딩 가능)
	UPROPERTY(BlueprintAssignable, Category = "MySQL")
	FOnMySQLConnectionResult OnConnectionResult;

public:
	// 데이터베이스 연결
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool ConnectToDatabase(const FString& host, int32 port, const FString& username, const FString& password, const FString& schema);

	// 데이터 삽입
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool InsertIntoDatabase(const FString& tablename, const FString& username, const FString& password);

	// 세션 종료
	UFUNCTION(BlueprintCallable, Category = "Database")
	void CloseDatabaseConnection();
		

private:
	// MySQLX Session
	mysqlx::Session* m_Session;
	mysqlx::Schema* m_SchemaDB;
};
