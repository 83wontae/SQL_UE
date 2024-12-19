// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <mysqlx/xdevapi.h> // MySQLX ��� ����
#include "MySQLComponent.generated.h"

// �������Ʈ�� ���� ���� ���ο� �޽����� ��ȯ�ϱ� ���� Delegate
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
	// �������Ʈ���� ȣ�� ������ MySQL ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "MySQL")
	void ConnectToDatabaseAsync(const FString& Host, int32 Port, const FString& Username, const FString& Password, const FString& Schema);

	// ���� ��� �̺�Ʈ (�������Ʈ���� ���ε� ����)
	UPROPERTY(BlueprintAssignable, Category = "MySQL")
	FOnMySQLConnectionResult OnConnectionResult;

public:
	// �����ͺ��̽� ����
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool ConnectToDatabase(const FString& host, int32 port, const FString& username, const FString& password, const FString& schema);

	// ������ ����
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool InsertIntoDatabase(const FString& tablename, const FString& username, const FString& password);

	// ���� ����
	UFUNCTION(BlueprintCallable, Category = "Database")
	void CloseDatabaseConnection();
		

private:
	// MySQLX Session
	mysqlx::Session* m_Session;
	mysqlx::Schema* m_SchemaDB;
};
