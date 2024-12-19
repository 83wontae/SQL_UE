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

UMySQLComponent::~UMySQLComponent()
{
    CloseDatabaseConnection();
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

void UMySQLComponent::ConnectToDatabaseAsync(const FString& Host, int32 Port, const FString& Username, const FString& Password, const FString& Schema)
{
    // 비동기로 MySQL 연결 작업 실행
    Async(EAsyncExecution::Thread, [this, Host, Port, Username, Password, Schema]()
    {
        std::string HostStr = TCHAR_TO_UTF8(*Host);
        std::string UsernameStr = TCHAR_TO_UTF8(*Username);
        std::string PasswordStr = TCHAR_TO_UTF8(*Password);

        try {
            // MySQL 세션 생성
            Session = new mysqlx::Session(HostStr, Port, UsernameStr, PasswordStr);

            // 스키마 설정
            // Session->sql("USE " + std::string(TCHAR_TO_UTF8(*Schema))).execute();

            // 연결 성공 시 메인 쓰레드에서 성공 이벤트 호출
            Async(EAsyncExecution::TaskGraphMainThread, [this, Schema]()
                {
                    OnConnectionResult.Broadcast(true, FString::Printf(TEXT("Connected to MySQL database and schema: %s"), *Schema));
                    UE_LOG(LogTemp, Log, TEXT("Connected to MySQL database and schema: %s"), *Schema);
                });
        }
        catch (const mysqlx::Error& Err) {
            // 연결 실패 시 메인 쓰레드에서 실패 이벤트 호출
            Async(EAsyncExecution::TaskGraphMainThread, [this, Err]()
                {
                    OnConnectionResult.Broadcast(false, FString::Printf(TEXT("MySQL Error: %s"), *FString(Err.what())));
                    UE_LOG(LogTemp, Error, TEXT("MySQL Error: %s"), *FString(Err.what()));
                });
        }
        catch (const std::exception& Ex) {
            // 일반 예외 발생 시
            Async(EAsyncExecution::TaskGraphMainThread, [this, Ex]()
                {
                    OnConnectionResult.Broadcast(false, FString::Printf(TEXT("Standard Exception: %s"), *FString(Ex.what())));
                    UE_LOG(LogTemp, Error, TEXT("Standard Exception: %s"), *FString(Ex.what()));
                });
        }
        catch (...) {
            // 알 수 없는 예외 처리
            Async(EAsyncExecution::TaskGraphMainThread, [this]()
                {
                    OnConnectionResult.Broadcast(false, TEXT("Unknown error occurred while connecting to MySQL."));
                    UE_LOG(LogTemp, Error, TEXT("Unknown error occurred while connecting to MySQL."));
                });
        }
    });
}

bool UMySQLComponent::ConnectToDatabase(const FString& host, int32 port, const FString& username, const FString& password, const FString& schema)
{
    std::string HostStr = TCHAR_TO_UTF8(*host);
    std::string UsernameStr = TCHAR_TO_UTF8(*username);
    std::string PasswordStr = TCHAR_TO_UTF8(*password);

    try {
        // MySQLX 세션 생성
        Session = new mysqlx::Session(HostStr, port, UsernameStr, PasswordStr);
        UE_LOG(LogTemp, Log, TEXT("Connected to MySQL database: %s"), *schema);

        // DefaultSchema 설정
        // Session->sql("USE " + std::string(TCHAR_TO_UTF8(*schema))).execute();
        return true;
    }
    catch (const mysqlx::Error& err) {
        UE_LOG(LogTemp, Error, TEXT("MySQL Error: %s"), *FString(err.what()));
        return false;
    }
    catch (std::exception& ex) {
        UE_LOG(LogTemp, Error, TEXT("Standard Exception: %s"), *FString(ex.what()));
        return false;
    }
    catch (...) {
        UE_LOG(LogTemp, Error, TEXT("Unknown exception occurred while connecting to MySQL."));
        return false;
    }
}

bool UMySQLComponent::InsertIntoDatabase(const FString& schema, const FString& tablename, const FString& username, const FString& password)
{
    if (!Session)
    {
        UE_LOG(LogTemp, Error, TEXT("No database session available. Connect to the database first."));
        return false;
    }

    try
    {
        // 스키마와 테이블 접근
        //mysqlx::Schema Schema = Session->getDefaultSchema();
        UE_LOG(LogTemp, Log, TEXT("Get Database Schema: %s"), *schema);
        mysqlx::Schema SchemaDB = Session->getSchema(TCHAR_TO_UTF8(*schema));
        UE_LOG(LogTemp, Log, TEXT("Get Tablename: %s"), *tablename);
        mysqlx::Table Table = SchemaDB.getTable(TCHAR_TO_UTF8(*tablename));

        // 데이터 삽입
        Table.insert("username", "password").values(TCHAR_TO_UTF8(*username), TCHAR_TO_UTF8(*password)).execute();
        UE_LOG(LogTemp, Log, TEXT("Data inserted successfully: Username = %s, Password = %d"), *username, *password);

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