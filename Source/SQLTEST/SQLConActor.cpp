// Fill out your copyright notice in the Description page of Project Settings.


#include "SQLConActor.h"
#include "Engine.h"
#define WIN32_LEAN_AND_MEAN
#include "include/mysql.h"
// Sets default values
ASQLConActor::ASQLConActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASQLConActor::BeginPlay()
{
	Super::BeginPlay();
	
	MYSQL* connection = nullptr;
	MYSQL conn;
	
	mysql_init(&conn);
	//connection = mysql_real_connect(&conn, "192.168.1.24", "user", "111111", "new_schema", 3306, (char *)NULL, 0);
	connection = mysql_real_connect(&conn, "mysql-testseraver.mysql.database.azure.com", "master@mysql-testseraver", "dnjsxo1!", "new_schema", 3306, (char *)NULL, 0);
	if (connection == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, TEXT("Connect Fail!"));
		FString err = mysql_error(&conn);
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, err);
		//exit(1);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, TEXT("Connect Success!"));

		//SELECT aaa FROM new_schema.new_table where ccc  = 1;

		MYSQL_RES *result = nullptr;
		MYSQL_ROW row;

		mysql_query(connection, "SELECT aaa FROM new_schema.new_table where ccc  = 4;");
		result = mysql_store_result(connection);

		int field = mysql_num_fields(result);
		
		row = mysql_fetch_row(result);
		while (row)
		{
			for (int i = 0; i < field; ++i)
			{
				GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, row[i]);
			}

			row = mysql_fetch_row(result);
		}
		

		mysql_close(connection);
	}
}

// Called every frame
void ASQLConActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

