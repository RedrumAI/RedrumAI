// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIGameMode.h"
#include "Manager/RAIHttpManager.h"
#include "Manager/RAIChatManager.h"

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


}

void ARAIGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(HttpManager))
	{
		HttpManager = GetWorld()->SpawnActor<ARAIHttpManager>(ARAIHttpManager::StaticClass());
	}

	BindHM2CM();
	//HttpManager->SendRequestToOpenAI(FString::Printf(TEXT("Say just Hi")));

}

void ARAIGameMode::BindHM2CM()
{
	if (IsValid(HttpManager))
	{
		//TODO: HM�� ��������Ʈ Ʈ���ſ� ���ε��ϱ�
		HttpManager->ResponseDelegate_NLP.AddDynamic(ChatManager, &ARAIChatManager::SetEmotionScore);
	}
	else
	{
		//TODO: Ÿ�̸Ӹ� ���� 0.1�� �ڿ� BindHM() �ٽ� ����.
		GetWorldTimerManager().SetTimer(
			TimerHandle_BindHM,
			this,
			&ARAIGameMode::BindHM2CM,
			0.1f,
			false
		);
	}
}