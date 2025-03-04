// Fill out your copyright notice in the Description page of Project Settings.


#include "MySQLComponent.h"

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

void UMySQLComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (m_Connection)
	{
		m_Connection->close(); // MySQL ¿¬°á ´Ý±â
		delete m_Connection;
		m_Connection = nullptr;
		UE_LOG(LogTemp, Error, TEXT("Database connection closed"));
	}
}

void UMySQLComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

bool UMySQLComponent::ConnectToDatabase(const FString& host, int32 port, const FString& username, const FString& password)
{
	std::string HostStr = TCHAR_TO_UTF8(*host);
	std::string UsernameStr = TCHAR_TO_UTF8(*username);
	std::string PasswordStr = TCHAR_TO_UTF8(*password);
	std::string ConnStr = "tcp://" + HostStr + ":" + std::to_string(port);

	try
	{
		m_Driver = sql::mysql::get_mysql_driver_instance();
		m_Connection = m_Driver->connect(ConnStr, UsernameStr, PasswordStr);

		if (m_Connection->isValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Connect Success!"));
			if (EventDBConnected.IsBound())
				EventDBConnected.Broadcast();
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Connection is invalid!"));
		}
	}
	catch (const sql::SQLException& err)
	{
		UE_LOG(LogTemp, Error, TEXT("MySQL Error: %s"), *FString(err.what()));
	}
	catch (std::exception& ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Standard Exception: %s"), *FString(ex.what()));
	}
	catch (...)
	{
		UE_LOG(LogTemp, Error, TEXT("Unknown exception occurred while connecting to MySQL."));
	}

	return false;
}

bool UMySQLComponent::InsertIntoDatabase(const FString& tablename, const FString& username, const FString& password)
{
	try
	{
		std::string strTableName = TCHAR_TO_UTF8(*tablename);
		std::string strUsername = TCHAR_TO_UTF8(*username);
		std::string strPassword = TCHAR_TO_UTF8(*password);

		std::string Query = "INSERT INTO " + strTableName + " (username, password) VALUES (?, ?)";
		std::unique_ptr<sql::PreparedStatement> pstmt(m_Connection->prepareStatement(Query));
		pstmt->setString(1, strUsername);
		pstmt->setString(2, strPassword);
		pstmt->executeUpdate();

		UE_LOG(LogTemp, Error, TEXT("Insert Success!"));
		return true;
	}
	catch (const sql::SQLException& err)
	{
		UE_LOG(LogTemp, Error, TEXT("Data insertion failed: %s"), *FString(err.what()));
	}
	catch (...)
	{
		UE_LOG(LogTemp, Error, TEXT("Data Insert Unknown Exception Error!!!"));
	}

	return false;
}

bool UMySQLComponent::SelectFromDatabase(const FString& tablename, const FString& username, const FString& password)
{
	try
	{
		std::string strTableName = TCHAR_TO_UTF8(*tablename);
		std::string strUsername = TCHAR_TO_UTF8(*username);
		std::string strPassword = TCHAR_TO_UTF8(*password);

		std::string Query = "SELECT username, password FROM " + strTableName + " WHERE username = ? AND password = ?";
		std::unique_ptr<sql::PreparedStatement> pstmt(m_Connection->prepareStatement(Query));
		pstmt->setString(1, strUsername);
		pstmt->setString(2, strPassword);

		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		if (!res->next())
		{
			UE_LOG(LogTemp, Error, TEXT("No data found!"));
			return false;
		}

		return true;
	}
	catch (const sql::SQLException& err)
	{
		UE_LOG(LogTemp, Error, TEXT("Data selection failed: %s"), *FString(err.what()));
	}
	catch (std::exception& ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Standard Exception: %s"), *FString(ex.what()));
	}
	catch (...)
	{
		UE_LOG(LogTemp, Error, TEXT("Data Select Unknown Exception Error!!!"));
	}

	return false;
}

bool UMySQLComponent::UseSchema(const FString& schema)
{
	if (!m_Connection)
	{
		UE_LOG(LogTemp, Error, TEXT("Connection is nullptr!"));
		return false;
	}

	try
	{
		std::string strSchema = TCHAR_TO_UTF8(*schema);
		std::string Query = "USE " + strSchema;
		std::unique_ptr<sql::Statement> stmt(m_Connection->createStatement());
		stmt->execute(Query);
		return true;
	}
	catch (const sql::SQLException& err)
	{
		UE_LOG(LogTemp, Error, TEXT("Schema selection failed: %s"), *FString(err.what()));
	}
	catch (std::exception& ex)
	{
		UE_LOG(LogTemp, Error, TEXT("Standard Exception: %s"), *FString(ex.what()));
	}
	catch (...)
	{
		UE_LOG(LogTemp, Error, TEXT("Schema Select Unknown Exception Error!!!"));
	}

	return false;
}
