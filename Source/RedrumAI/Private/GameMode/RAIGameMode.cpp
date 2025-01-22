// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIGameMode.h"
//Secrets.ini 경로 탐지 및 ini로부터 읽어오는 헤더. CoreMinimal에 Path관련 코드가 있기에 없어도된다.
/*
#include "Misc/ConfigCacheIni.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
*/

#include "Manager/RAIHttpManager.h"

//Secretes.ini로부터 API_KEY 불러오는 예시코드
ARAIGameMode::ARAIGameMode()
{
	/*
	// Secrets.ini 경로 지정
	FString SecretsPath = FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("Secrets.ini"));
	GConfig->LoadFile(SecretsPath);

	FString OpenAI_API_Key;
	if (GConfig->GetString(TEXT("OpenAI"), TEXT("OpenAI_API_KEY"), OpenAI_API_Key, SecretsPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("OpenAI_API_KEY Loaded: %s"), *OpenAI_API_Key);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load OpenAI_API_KEY from Secrets"));
	}

	FString HF_API_Key;
	if (GConfig->GetString(TEXT("HuggingFace"), TEXT("HF_API_KEY"), HF_API_Key, SecretsPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("HF_API_KEY Loaded: %s"), *HF_API_Key);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load HF_API_KEY from Secrets"));
	}
	*/

	URAIHttpManager* HttpManager;
	HttpManager = NewObject<URAIHttpManager>();

	HttpManager->SendRequestToOpenAI(FString::Printf(TEXT("Say just Hi")));

}
