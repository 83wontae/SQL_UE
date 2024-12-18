// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <mysqlx/xdevapi.h> // MySQLX 헤더 파일
#include "MySQLComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALWITHMYSQL_API UMySQLComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMySQLComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 데이터베이스 연결
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool ConnectToDatabase(const FString& Host, int32 Port, const FString& Username, const FString& Password, const FString& Schema);

	// 데이터 삽입
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool InsertIntoDatabase(const FString& TableName, const FString& Name, const FString& Password);

	// 세션 종료
	UFUNCTION(BlueprintCallable, Category = "Database")
	void CloseDatabaseConnection();
		

private:
	// MySQLX Session
	mysqlx::Session* Session;
};
