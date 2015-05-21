// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "PhoneGame.h"
#include "PhoneCharacter.h"
#include "AllowWindowsPlatformTypes.h"
//#include "Networking.h"
//#include "Sockets.h"
//#include "SocketSubsystem.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <thread>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include "HideWindowsPlatformTypes.h"

struct PhoneData;
union Datum;
void parseDatagram(char[]);
float getFloat(char[], int);
float receive();

union Datum{
	char b[4];
	float f;
};

struct PhoneData{
	FVector gyro;
};

std::thread pollingThread;
float ax; float ay; float az;
float gx; float gy; float gz;
float mx; float my; float mz;
float GoogleYaw; float GooglePitch; float GoogleRoll;
bool killThread;

float getFloat(char buffer[], int offset)
{
	Datum result;
	result.b[0] = buffer[offset]; result.b[1] = buffer[offset + 1]; result.b[2] = buffer[offset + 2]; result.b[3] = buffer[offset + 3];
	return result.f;
}

// Sets default values
APhoneCharacter::APhoneCharacter(const FObjectInitializer& ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

// Called when the game starts or when spawned
void APhoneCharacter::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Gamee Starto"));
	std::thread pollingThread(receive);
	pollingThread.detach();

	Super::BeginPlay();

}

void APhoneCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason){
	//pollingThread.~thread();
	killThread = true;
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void APhoneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APhoneCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

FVector APhoneCharacter::getGyroVector(){
	//return data.gyro;
	//UE_LOG(LogTemp, Log, TEXT("Getting Gyro Vector"));
	return FVector(gx, gy, gz);
	//return FVector(1, 2, 3);
}

void parseDatagram(char buffer[]){
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Blue, TEXT("Parsing Datagram"));
	UE_LOG(LogTemp, Log, TEXT("Parsing Datagram"));

	//bytes 0,1 encode raw and orientation, except it seems raw is byte 1 bit 0 and orientation byte 1 bit 1
	//Raw = 0x01, Orientation = 0x02
	//delta time is calculated here as time since last packet and passed into smoothing
	//seems data is 4 byte floats
	//raw data is the next 36 bytes
	/*var ax = Raw.ax = GetFloat(bytes, index, 0);	var ay = Raw.ay = GetFloat(bytes, index, 4);	var az = Raw.az = GetFloat(bytes, index, 8);
	var gx = Raw.gx = GetFloat(bytes, index, 12);	var gy = Raw.gy = GetFloat(bytes, index, 16);	var gz = Raw.gz = GetFloat(bytes, index, 20);
	var mx = Raw.mx = GetFloat(bytes, index, 24);	var my = Raw.my = GetFloat(bytes, index, 28);	var mz = Raw.mz = GetFloat(bytes, index, 32);*/
	//orientation is the following 12 bytes
	/*GoogleYaw = GetFloat(bytes, index, 0);	GooglePitch = GetFloat(bytes, index, 4);	GoogleRoll = GetFloat(bytes, index, 8);*/

	signed char flags = buffer[1];
	char hasRaw = flags & 0x01;
	flags = flags >> 1;
	char hasOrientation = flags & 0x01;

	int index = 2;
	ax = getFloat(buffer, index + 0);
	ay = getFloat(buffer, index + 4);
	az = getFloat(buffer, index + 8);
	gx = getFloat(buffer, index + 12);
	gy = getFloat(buffer, index + 16);
	gz = getFloat(buffer, index + 20);

	//data.gyro = FVector(gx, gy, gz);

	mx = getFloat(buffer, index + 24);
	my = getFloat(buffer, index + 28);
	mz = getFloat(buffer, index + 32);

	index += 36;
	GoogleYaw = getFloat(buffer, index + 0);
	GooglePitch = getFloat(buffer, index + 4);
	GoogleRoll = getFloat(buffer, index + 8);

	std::cout << "hasOrientation is " << (int)hasOrientation << " and hasRaw is " << (int)hasRaw << std::endl;
	std::cout << "ax " << ax << " ay " << ay << " az " << az << std::endl;
	std::cout << "GoogleYaw " << GoogleYaw << " GooglePitch " << GooglePitch << " GoogleRoll " << GoogleRoll << std::endl;

	//getfloat here
	FString result = FString::SanitizeFloat(gy);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, *result);
}



float receive(){

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Threado Starto"));
	UE_LOG(LogTemp, Log, TEXT("Threado Starto"));
	//while (true){
	//	if (killThread) return 0;
	//killThread = true;
	//}
	//while (true){ GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, TEXT("tick")); }


	SOCKET sd;
	unsigned short int port = 5555;
	struct sockaddr_in server, client;
	WSADATA wsaData;

	const int BUFFER_SIZE = 8192;
	char buffer[BUFFER_SIZE];

	/* Open windows connection */
	if (WSAStartup(0x0101, &wsaData) != 0)//variable w is a structure of WSADATA form.
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Could not open Windows connection"));
		fprintf(stderr, "Could not open Windows connection.\n");
		UE_LOG(LogTemp, Log, TEXT("Could not open Windows connection.\n"));
		return 1;
	}

	/* Open a datagram socket */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == INVALID_SOCKET)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Could not create socket"));
		fprintf(stderr, "Could not create socket.\n");
		UE_LOG(LogTemp, Log, TEXT("Could not create socket.\n"));
		WSACleanup();
		return 2;
	}

	/* Clear out server struct */
	memset((void *)&server, '\0', sizeof(struct sockaddr_in));

	/* Set family and port */
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	/* Set address automatically if desired */

	/* automatically get host name of this computer */
	//int errorCode;
	char host_name[256];
	gethostname(host_name, sizeof(host_name));
	hostent *hp = gethostbyname(host_name);

	

	/* Check for NULL pointer */
	if (hp != NULL)
	{
		/* Assign the address */
		server.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr_list[0][0];
		server.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr_list[0][1];
		server.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr_list[0][2];
		server.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr_list[0][3];

		FString Fs = FString(ANSI_TO_TCHAR(host_name));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Fs);
		std::cout << "Hostname: " << host_name << " " << ((int)hp->h_addr_list[0][0] & 0x256) << "." << (unsigned int)hp->h_addr_list[0][1] << "." << (unsigned int)hp->h_addr_list[0][2] << "." << (unsigned int)hp->h_addr_list[0][3] << std::endl;
	}

	/* hardcoded address as backup */
	else
	{
		fprintf(stderr, "Failed to discover hostname. Defaulted to 192.168.1.107. \n");
		UE_LOG(LogTemp, Log, TEXT("Failed to discover hostname. Defaulted to 192.168.1.104. \n"));
		server.sin_addr.S_un.S_un_b.s_b1 = (unsigned char)192;
		server.sin_addr.S_un.S_un_b.s_b2 = (unsigned char)168;
		server.sin_addr.S_un.S_un_b.s_b3 = (unsigned char)1;
		server.sin_addr.S_un.S_un_b.s_b4 = (unsigned char)104;
	}

	//doesn't crash when stalled here, bind seems to be the tricky part

	/* Bind address to socket */
	if (bind(sd, (struct sockaddr *)&server,
		sizeof(struct sockaddr_in)) == -1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Could not bind address to socket"));
		fprintf(stderr, "Could not bind address to socket.\n");
		UE_LOG(LogTemp, Log, TEXT("Could not bind address to socket.\n"));
		closesocket(sd);
		WSACleanup();
		return 3;
	}

	int client_length = (int)sizeof(struct sockaddr_in);
	int bytes_received;

	//while (true){
//		if (killThread) return 0;
	//}

	while (true){
		UE_LOG(LogTemp, Log, TEXT("KillThread is %killThread"));
		if (killThread == true) {
			fprintf(stderr, "KillThread: ending server for FreePIE.\n");
			UE_LOG(LogTemp, Log, TEXT("KillThread: ending server for FreePIE.\n"));
			closesocket(sd);
			WSACleanup();
			return 5555;
		}
		//}
		//std::cout << "Reading a datagram" << std::endl;
		//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Black, TEXT("about to read")); //GEngine->
		/* Receive bytes from client */
		bytes_received = recvfrom(sd, buffer, BUFFER_SIZE, 0,
			(struct sockaddr *)&client, &client_length);
		/*if (bytes_received < 0)
		{
		//while (true){
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Black, TEXT("Failed to receive datagram"));
		//}
		fprintf(stderr, "Could not receive datagram.\n");
		closesocket(sd);
		WSACleanup();
		//pollingThread.~thread();
		//ExitThread(0);
		return 4444;
		}*/

		int nError = WSAGetLastError();
		if (nError != WSAEWOULDBLOCK&&nError != 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Winsock error code\r\n"));
			std::cout << "Winsock error code: " << nError << "\r\n";
			std::cout << "Server disconnected!\r\n";
			return 9000;
		}

		else if (bytes_received > 0) parseDatagram((char*)buffer);
		//std::cout << (int)client.sin_addr.S_un.S_un_b.s_b1 << '.' << (int)client.sin_addr.S_un.S_un_b.s_b2 << '.' << (int)client.sin_addr.S_un.S_un_b.s_b3 << '.' << (int)client.sin_addr.S_un.S_un_b.s_b4 << std::endl;
		//fprintf(stderr, buffer);
	}
	return 2;
}