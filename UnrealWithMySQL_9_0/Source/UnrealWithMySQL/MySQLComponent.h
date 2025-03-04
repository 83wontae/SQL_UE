// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma push_macro("check")
#undef check  // Unreal의 check 매크로 해제

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>

#pragma pop_macro("check")  // check 매크로 원래 상태로 복원

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MySQLComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDBConnected);

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

	/**
	 * Ends gameplay for this component.
	 * Called from AActor::EndPlay only if bHasBegunPlay is true
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	bool ConnectToDatabase(const FString& host, int32 port, const FString& username, const FString& password);

	UFUNCTION(BlueprintCallable)
	bool InsertIntoDatabase(const FString& tablename, const FString& username, const FString& password);

	UFUNCTION(BlueprintCallable)
	bool SelectFromDatabase(const FString& tablename, const FString& username, const FString& password);

	UFUNCTION(BlueprintCallable)
	bool UseSchema(const FString& schema);

private:
	sql::Driver* m_Driver;
	sql::Connection* m_Connection;

	UPROPERTY(BlueprintAssignable, Category = "MySQL")
	FOnDBConnected EventDBConnected;
};
