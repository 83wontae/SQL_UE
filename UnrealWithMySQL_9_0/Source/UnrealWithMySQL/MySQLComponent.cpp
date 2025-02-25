// Fill out your copyright notice in the Description page of Project Settings.


#include "MySQLComponent.h"
#include <xdevapi.h> // MySQLX ��� ����

// Sets default values for this component's properties
UMySQLComponent::UMySQLComponent():m_Session(nullptr), m_SchemaDB(std::nullopt)
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

void UMySQLComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogTemp, Log, TEXT("EndPlay MySQLComponent..."));

    {
        FScopeLock Lock(&SessionCriticalSection);

        if (m_Session)
        {
            try
            {
                UE_LOG(LogTemp, Log, TEXT("Closing MySQL session..."));
                m_Session->close(); // MySQL ���� ����
                m_Session.Reset(); // TUniquePtr ����
                UE_LOG(LogTemp, Log, TEXT("MySQL session closed successfully."));
            }
            catch (const mysqlx::Error& Err)
            {
                UE_LOG(LogTemp, Error, TEXT("Error while closing session: %s"), *FString(Err.what()));
            }
        }
    }

    // ��� Ÿ�̸� ����
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    }

    Super::EndPlay(EndPlayReason);
}

// Called every frame
void UMySQLComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UMySQLComponent::ConnectToDatabase(const FString& host, int32 port, const FString& username, const FString& password, const FString& schema)
{
    std::string HostStr = TCHAR_TO_UTF8(*host);
    std::string UsernameStr = TCHAR_TO_UTF8(*username);
    std::string PasswordStr = TCHAR_TO_UTF8(*password);

    try {
        {
            FScopeLock Lock(&SessionCriticalSection);

            // MySQL ���� ����
            m_Session = MakeUnique<mysqlx::Session>(HostStr, port, UsernameStr, PasswordStr);
        }

        // ��Ű���� �⺻������ ����
        m_SchemaDB = m_Session->getSchema(TCHAR_TO_UTF8(*schema));

        OnConnectionResult.Broadcast(true, FString::Printf(TEXT("Connected to MySQL database and schema: %s"), *schema));
        UE_LOG(LogTemp, Log, TEXT("Connected to MySQL database and schema: %s"), *schema);

        return true;
    }
    catch (const mysqlx::Error& err) {

        OnConnectionResult.Broadcast(false, FString::Printf(TEXT("MySQL Error: %s"), *FString(err.what())));
        UE_LOG(LogTemp, Error, TEXT("MySQL Error: %s"), *FString(err.what()));
        return false;
    }
    catch (std::exception& ex) {

        OnConnectionResult.Broadcast(false, FString::Printf(TEXT("Standard Exception: %s"), *FString(ex.what())));
        UE_LOG(LogTemp, Error, TEXT("Standard Exception: %s"), *FString(ex.what()));
        return false;
    }
    catch (...) {

        OnConnectionResult.Broadcast(false, TEXT("Unknown error occurred while connecting to MySQL."));
        UE_LOG(LogTemp, Error, TEXT("Unknown exception occurred while connecting to MySQL."));
        return false;
    }
}

bool UMySQLComponent::InsertIntoDatabase(const FString& tablename, const FString& username, const FString& password)
{
    if (!m_Session)
    {
        UE_LOG(LogTemp, Error, TEXT("No database session available. Connect to the database first."));
        return false;
    }

    try
    {
        // ��Ű���� ���̺� ����
        // UE_LOG(LogTemp, Log, TEXT("Get Database Schema: %s"), *schema);
        // mysqlx::Schema SchemaDB = m_Session->getDefaultSchema();
        // mysqlx::Schema SchemaDB = m_Session->getSchema(TCHAR_TO_UTF8(*schema));
        UE_LOG(LogTemp, Log, TEXT("Get Tablename: %s"), *tablename);
        mysqlx::Table Table = m_SchemaDB->getTable(TCHAR_TO_UTF8(*tablename));

        // ������ ����
        Table.insert("username", "password").values(TCHAR_TO_UTF8(*username), TCHAR_TO_UTF8(*password)).execute();
        UE_LOG(LogTemp, Log, TEXT("Data inserted successfully: Username = %s, Password = %s"), *username, *password);

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

bool UMySQLComponent::SelectIntoDatabase(const FString& tablename, const FString& username, const FString& password)
{
    if (IsSessionValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Database schema is not initialized."));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Username: %s, Password: %s"), *username, *password);

    if (tablename.IsEmpty() || username.IsEmpty() || password.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Tablename or Username or password is empty!"));
        return false;
    }

    try
    {
        // ���̺� ��ü ��������
        mysqlx::Table Table = m_SchemaDB->getTable(TCHAR_TO_UTF8(*tablename));

        if (!Table.existsInDatabase())
        {
            UE_LOG(LogTemp, Error, TEXT("Table does not exist in database: %s"), *tablename);
            return false;
        }

        // SELECT ���� ����: username�� password�� ��ġ�ϴ� ���ڵ� ��ȸ
        // SQL ���� : SELECT username, password FROM users WHERE username = 'username' AND password = 'password';
        mysqlx::RowResult Result = Table.select("username", "password")
            .where("username = :uname AND password = :pwd")
            .bind("uname", TCHAR_TO_UTF8(*username))
            .bind("pwd", TCHAR_TO_UTF8(*password))
            .execute();

        
        // ��� Ȯ��
        if (Result.count() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid credentials for user: %s"), *username);
            return false; // �����Ͱ� ������ false ��ȯ
        }

        mysqlx::Row RowData = Result.fetchOne();// Row ���� Read

        // RowData �������� �ʱ�ȭ (�ʼ��� �ƴ�)
        RowData = mysqlx::Row();  // ��������� �ʱ�ȭ

        UE_LOG(LogTemp, Log, TEXT("User credentials are valid: %s"), *username);
        return true; // �����Ͱ� �����ϸ� true ��ȯ
    }
    catch (const mysqlx::Error& err)
    {
        UE_LOG(LogTemp, Error, TEXT("MySQL Error: %s"), *FString(err.what()));
        return false;
    }
    catch (std::exception& ex)
    {
        UE_LOG(LogTemp, Error, TEXT("Standard Exception: %s"), *FString(ex.what()));
        return false;
    }
    catch (...)
    {
        UE_LOG(LogTemp, Error, TEXT("Unknown error occurred while validating credentials."));
        return false;
    }
}

bool UMySQLComponent::IsSessionValid()
{
    if (!m_Session)
    {
        UE_LOG(LogTemp, Error, TEXT("Session is null."));
        return false;
    }

    try
    {
        mysqlx::SqlStatement Result = m_Session->sql("SELECT 1");
        Result.execute();
        return true; // ���� ���� ���� = ������ ��ȿ��
    }
    catch (const mysqlx::Error& Err)
    {
        UE_LOG(LogTemp, Error, TEXT("Session validation failed: %s"), *FString(Err.what()));
        return false;
    }
    catch (...)
    {
        UE_LOG(LogTemp, Error, TEXT("Unknown error during session validation."));
        return false;
    }
}
