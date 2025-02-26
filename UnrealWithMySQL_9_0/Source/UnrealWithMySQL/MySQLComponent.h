// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <mysqlx/xdevapi.h> // MySQLX ��� ����
#include "HAL/CriticalSection.h"
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// �����ͺ��̽� ����
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool ConnectToDatabase(const FString& host, int32 port, const FString& username, const FString& password, const FString& schema);

	// ������ ����
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool InsertIntoDatabase(const FString& tablename, const FString& username, const FString& password);

	// Select ������ ����(username, password�� �����ϸ� ���̺� �ȿ� �����͸� Ȯ���Ͽ� �����ϸ� True, �������� ������ False ��ȯ)
	UFUNCTION(BlueprintCallable, Category = "Database")
	bool SelectIntoDatabase(const FString& tablename, const FString& username, const FString& password);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool IsSessionValid();

private:
	FCriticalSection SessionCriticalSection;

	// MySQLX Session
	TUniquePtr<mysqlx::Session> m_Session;
	// std::unique_ptr<mysqlx::Session> m_Session;


	// std::optional<T>�� C++17���� �߰��� ���� ���� ���� �ִ� ������ �����ϰ� �����ϴ� �����̳��Դϴ�.
	// ��, ���� ���� ���� �ְ� ���� ���� �ִ� ��Ȳ���� nullptr�̳� ���� ó���� ���� �ʰ� �� ������ ����� �����մϴ�.
	// TOptional<mysqlx::Schema> m_SchemaDB; // optional�� �����Ͽ� ������ ����
	FString m_SchemaName;

	// ���� ��� �̺�Ʈ (�������Ʈ���� ���ε� ����)
	UPROPERTY(BlueprintAssignable, Category = "MySQL")
	FOnMySQLConnectionResult OnConnectionResult;
};
