// Fill out your copyright notice in the Description page of Project Settings.


#include "MySQLComponent.h"
#include <xdevapi.h> // MySQLX ��� ����

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
    // �񵿱�� MySQL ���� �۾� ����
    Async(EAsyncExecution::Thread, [this, Host, Port, Username, Password, Schema]()
    {
        std::string HostStr = TCHAR_TO_UTF8(*Host);
        std::string UsernameStr = TCHAR_TO_UTF8(*Username);
        std::string PasswordStr = TCHAR_TO_UTF8(*Password);

        try {
            // MySQL ���� ����
            Session = new mysqlx::Session(HostStr, Port, UsernameStr, PasswordStr);

            // ��Ű�� ����
            // Session->sql("USE " + std::string(TCHAR_TO_UTF8(*Schema))).execute();

            // ���� ���� �� ���� �����忡�� ���� �̺�Ʈ ȣ��
            Async(EAsyncExecution::TaskGraphMainThread, [this, Schema]()
                {
                    OnConnectionResult.Broadcast(true, FString::Printf(TEXT("Connected to MySQL database and schema: %s"), *Schema));
                    UE_LOG(LogTemp, Log, TEXT("Connected to MySQL database and schema: %s"), *Schema);
                });
        }
        catch (const mysqlx::Error& Err) {
            // ���� ���� �� ���� �����忡�� ���� �̺�Ʈ ȣ��
            Async(EAsyncExecution::TaskGraphMainThread, [this, Err]()
                {
                    OnConnectionResult.Broadcast(false, FString::Printf(TEXT("MySQL Error: %s"), *FString(Err.what())));
                    UE_LOG(LogTemp, Error, TEXT("MySQL Error: %s"), *FString(Err.what()));
                });
        }
        catch (const std::exception& Ex) {
            // �Ϲ� ���� �߻� ��
            Async(EAsyncExecution::TaskGraphMainThread, [this, Ex]()
                {
                    OnConnectionResult.Broadcast(false, FString::Printf(TEXT("Standard Exception: %s"), *FString(Ex.what())));
                    UE_LOG(LogTemp, Error, TEXT("Standard Exception: %s"), *FString(Ex.what()));
                });
        }
        catch (...) {
            // �� �� ���� ���� ó��
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
        // MySQLX ���� ����
        Session = new mysqlx::Session(HostStr, port, UsernameStr, PasswordStr);
        UE_LOG(LogTemp, Log, TEXT("Connected to MySQL database: %s"), *schema);

        // DefaultSchema ����
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
        // ��Ű���� ���̺� ����
        //mysqlx::Schema Schema = Session->getDefaultSchema();
        UE_LOG(LogTemp, Log, TEXT("Get Database Schema: %s"), *schema);
        mysqlx::Schema SchemaDB = Session->getSchema(TCHAR_TO_UTF8(*schema));
        UE_LOG(LogTemp, Log, TEXT("Get Tablename: %s"), *tablename);
        mysqlx::Table Table = SchemaDB.getTable(TCHAR_TO_UTF8(*tablename));

        // ������ ����
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