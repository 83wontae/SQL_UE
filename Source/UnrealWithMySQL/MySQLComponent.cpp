// Fill out your copyright notice in the Description page of Project Settings.


#include "MySQLComponent.h"
#include <xdevapi.h> // MySQLX 헤더 파일

// Sets default values for this component's properties
UMySQLComponent::UMySQLComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMySQLComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMySQLComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UMySQLComponent::ConnectToDatabase(const FString& Host, int32 Port, const FString& Username, const FString& Password, const FString& Schema)
{
    try
    {
        // MySQLX 세션 생성
        Session = new mysqlx::Session(TCHAR_TO_UTF8(*Host), Port, TCHAR_TO_UTF8(*Username), TCHAR_TO_UTF8(*Password));
        UE_LOG(LogTemp, Log, TEXT("Connected to MySQL database: %s"), *Schema);

        // 스키마 설정
        Session->getSchema(TCHAR_TO_UTF8(*Schema));
        return true;
    }
    catch (const mysqlx::Error& Err)
    {
        UE_LOG(LogTemp, Error, TEXT("Database connection failed: %s"), *FString(Err.what()));
        return false;
    }
    catch (...)
    {
        UE_LOG(LogTemp, Error, TEXT("An unknown error occurred while connecting to the database."));
        return false;
    }
}

bool UMySQLComponent::InsertIntoDatabase(const FString& TableName, const FString& Name, const FString& Password)
{
    if (!Session)
    {
        UE_LOG(LogTemp, Error, TEXT("No database session available. Connect to the database first."));
        return false;
    }

    try
    {
        // 스키마와 테이블 접근
        mysqlx::Schema Schema = Session->getDefaultSchema();
        mysqlx::Table Table = Schema.getTable(TCHAR_TO_UTF8(*TableName));

        // 데이터 삽입
        Table.insert("name", "age").values(TCHAR_TO_UTF8(*Name), TCHAR_TO_UTF8(*Name)).execute();
        UE_LOG(LogTemp, Log, TEXT("Data inserted successfully: Name = %s, Password = %d"), *Name, *Password);

        return true;
    }
    catch (const mysqlx::Error& Err)
    {
        UE_LOG(LogTemp, Error, TEXT("Data insertion failed: %s"), *FString(Err.what()));
        return false;
    }
    catch (...)
    {
        UE_LOG(LogTemp, Error, TEXT("An unknown error occurred while inserting data."));
        return false;
    }
}

void UMySQLComponent::CloseDatabaseConnection()
{
    if (Session)
    {
        delete Session;
        Session = nullptr;
        UE_LOG(LogTemp, Log, TEXT("Database session closed."));
    }
}