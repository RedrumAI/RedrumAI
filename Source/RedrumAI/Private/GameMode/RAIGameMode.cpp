// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIGameMode.h"
//Secrets.ini ��� Ž�� �� ini�κ��� �о���� ���. CoreMinimal�� Path���� �ڵ尡 �ֱ⿡ ����ȴ�.
/*
#include "Misc/ConfigCacheIni.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
*/

#include "Manager/RAIHttpManager.h"

//Secretes.ini�κ��� API_KEY �ҷ����� �����ڵ�
ARAIGameMode::ARAIGameMode()
{
	/*
	// Secrets.ini ��� ����
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
