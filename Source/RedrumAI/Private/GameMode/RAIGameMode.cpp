// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIGameMode.h"
#include "Manager/RAIHttpManager.h"
#include "Manager/RAIChatManager.h"

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
		//TODO: HM의 델리게이트 트리거와 바인드하기
		HttpManager->ResponseDelegate_NLP.AddDynamic(ChatManager, &ARAIChatManager::SetEmotionScore);
	}
	else
	{
		//TODO: 타이머를 통해 0.1초 뒤에 BindHM() 다시 실행.
		GetWorldTimerManager().SetTimer(
			TimerHandle_BindHM,
			this,
			&ARAIGameMode::BindHM2CM,
			0.1f,
			false
		);
	}
}