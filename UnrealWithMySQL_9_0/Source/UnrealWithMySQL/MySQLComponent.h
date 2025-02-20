// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <mysqlx/xdevapi.h> // MySQLX 헤더 파일
#include "HAL/CriticalSection.h"
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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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

	// Select 데이터 검증(username, password를 전달하면 테이블 안에 데이터를 확인하여 존재하면 True, 존재하지 않으면 False 반환)
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool SelectIntoDatabase(const FString& tablename, const FString& username, const FString& password);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool IsSessionValid();

private:
	FCriticalSection SessionCriticalSection;

	// MySQLX Session
	std::unique_ptr<mysqlx::Session> m_Session;


	// std::optional<T>는 C++17에서 추가된 값이 없을 수도 있는 변수를 안전하게 관리하는 컨테이너입니다.
	// 즉, 값이 있을 수도 있고 없을 수도 있는 상황에서 nullptr이나 예외 처리를 쓰지 않고 더 안전한 방법을 제공합니다.
	std::optional<mysqlx::Schema> m_SchemaDB; // optional로 관리하여 안전성 증가
};
