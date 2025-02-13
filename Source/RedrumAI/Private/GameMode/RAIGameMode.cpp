// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIGameMode.h"
#include "Manager/RAIHttpManager.h"
#include "Manager/RAIChatManager.h"

//Secretes.ini로부터 API_KEY 불러오는 예시코드
ARAIGameMode::ARAIGameMode()
{

}

void ARAIGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(HttpManager))
	{
		HttpManager = GetWorld()->SpawnActor<ARAIHttpManager>(ARAIHttpManager::StaticClass());
	}
	if (!IsValid(ChatManager))
	{
		ChatManager = GetWorld()->SpawnActor<ARAIChatManager>(ARAIChatManager::StaticClass());
	}

	BindHM2CM();

	FTimerHandle TimerHandle_tmp;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_tmp,
		this,
		&ARAIGameMode::tmpRequestNLP,
		3.0f,
		false
	);
}

void ARAIGameMode::tmpRequestNLP()
{
	FString str;
	str = FString::Printf(TEXT("Good Morning. Do you want some tea"));
	HttpManager->SendRequestToNLP(str);
}

void ARAIGameMode::BindHM2CM()
{
	if (IsValid(HttpManager) && IsValid(ChatManager))
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

