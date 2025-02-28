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

	BindHM();
	BindCM();

	FTimerHandle TimerHandle_tmp;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_tmp,
		this,
		&ARAIGameMode::tmpTimerFunction,
		3.0f,
		false
	);
	FTimerHandle TimerHandle2_tmp;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle2_tmp,
		this,
		&ARAIGameMode::tmpTimerFunction2,
		8.0f,
		false
	);
	FTimerHandle TimerHandle3_tmp;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle3_tmp,
		this,
		&ARAIGameMode::tmpTimerFunction3,
		13.0f,
		false
	);
}

void ARAIGameMode::tmpTimerFunction()
{
	FString str;
	str = FString::Printf(
		TEXT("우리는 역할극을 진행 중이야. 나는 형사이고 너는 살인범이야. ")
		TEXT("앞으로 내 메시지에는 너의 감정 상태 점수가 반드시 대괄호로 표시되어 있을 거야. ")
		TEXT("이 감정 점수는 순서대로 [애정, 즐거움, 놀라움, 분노, 공포, 슬픔]을 나타내며, ")
		TEXT("각 값은 0.00부터 0.99까지의 숫자로, 0.99에 가까울수록 해당 감정이 더욱 격해진다는 의미야. ")
		TEXT("하지만 네 응답에서는 절대로 감정 점수를 포함하지 마. ")
		TEXT("즉, 네 모든 응답은 감정 점수를 제거한 순수 텍스트만으로 이루어져야 해. ")
		TEXT("이전 메시지에 감정 점수가 있었다 하더라도 반드시 무시하고, 오직 응답 내용만 보내줘. ")
		TEXT("그리고 나의 첫 번째 시스템 메시지에 대해서는 응답으로 단지 \"!\"만 보내.")
	);
	if (IsValid(ChatManager) && HttpManager)
	{
		ChatManager->AddMessageArray(str, Developer);
	}
	//일단 이 developer메시지가 cm에 잘들어가는지만 확인해보자.
	//UE_LOG(LogTemp, Warning, TEXT("%s", ))
}

void ARAIGameMode::tmpTimerFunction2()
{
	FString str;
	str = FString::Printf(
		TEXT("오늘 아침에 너는 무슨일을 하고있었지?")
	);
	if (IsValid(ChatManager) && HttpManager)
	{
		ChatManager->AddMessageArray(str, User);
	}
}

void ARAIGameMode::tmpTimerFunction3()
{
	FString str;
	str = FString::Printf(
		TEXT("그래 천천히해보자. 네 이름이 뭐지? 이건 꼭 대답해.")
	);
	if (IsValid(ChatManager) && HttpManager)
	{
		ChatManager->AddMessageArray(str, User);
	}
}

void ARAIGameMode::BindHM()
{
	if (IsValid(HttpManager))
	{
		HttpManager->ResponseDelegate_NLP.AddDynamic(this, &ARAIGameMode::OnEventDelegate_NLP);
		HttpManager->ResponseDelegate_OpenAI.AddDynamic(this, &ARAIGameMode::OnEventDelegate_OpenAI);

		UE_LOG(LogTemp, Warning, TEXT("GM:BindHM Complete"));
	}
	else
	{
		FTimerHandle TimerHandle_BindHM;
		//TODO: 타이머를 통해 0.1초 뒤에 BindHM() 다시 실행.
		GetWorldTimerManager().SetTimer(
			TimerHandle_BindHM,
			this,
			&ARAIGameMode::BindHM,
			0.1f,
			false
		);
	}
}

void ARAIGameMode::BindCM()
{
	if (IsValid(ChatManager))
	{
		ChatManager->SendMessageDelegate.AddDynamic(this, &ARAIGameMode::OnEventDelegate_SendMessageArray);
		UE_LOG(LogTemp, Warning, TEXT("GM:BindCM Complete"));
	}
	else
	{
		FTimerHandle TimerHandle_BindCM;
		//TODO: 타이머를 통해 0.1초 뒤에 BindHM() 다시 실행.
		GetWorldTimerManager().SetTimer(
			TimerHandle_BindCM,
			this,
			&ARAIGameMode::BindCM,
			0.1f,
			false
		);
	}
}

void ARAIGameMode::OnEventDelegate_NLP(FString InJsonData)
{
	//언리얼엔진의 멀티쓰레드 환경을 고려해 OnEventDelegate_OpenAI의 CM->AddMessageArray가 작동하는 것을 방지하기 위해 단일쓰레드 강제사용
	AsyncTask(ENamedThreads::GameThread, [this, InJsonData]()
	{
		ScoreString = InJsonData;
		if (ScoreString.IsSet() && ResponseString.IsSet())
		{
			ChatManager->AddMessageArray(ScoreString.GetValue(), ResponseString.GetValue(), Assistant);
			ScoreString.Reset();
			ResponseString.Reset();
		}
	});
}


void ARAIGameMode::OnEventDelegate_OpenAI(FString Message)
{
	//언리얼엔진의 멀티쓰레드 환경을 고려해 OnEventDelegate_NLP의 CM->AddMessageArray가 작동하는 것을 방지하기 위해 단일쓰레드 강제사용
	AsyncTask(ENamedThreads::GameThread, [this, Message]()
	{
		ResponseString = Message;
		if (ScoreString.IsSet() && ResponseString.IsSet())
		{
			ChatManager->AddMessageArray(ScoreString.GetValue(), ResponseString.GetValue(), Assistant);
			ScoreString.Reset();
			ResponseString.Reset();
		}
	});
}

void ARAIGameMode::OnEventDelegate_SendMessageArray(FString MessageString)
{
	HttpManager->SendRequestToOpenAI(MessageString);
}
