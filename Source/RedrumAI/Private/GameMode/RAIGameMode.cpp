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

	FTimerHandle TimerHandle_tmp;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_tmp,
		this,
		&ARAIGameMode::tmpTestFunction,
		3.0f,
		false
	);
}

void ARAIGameMode::tmpTestFunction()
{
	FString str;
	str = FString::Printf(
		TEXT("우린 역할극 중이야. 나는 형사고 너는 살인범이지.")
		TEXT("지금부터 우리의 대화내역을 보낼건데, 나의 Role : user 메시지 맨 앞에 너의 감정상태를 점수로 대괄호 안에 담을거야.")
		TEXT("그 점수에 맞춰 대답을 해줘.각 대괄호는[애정, 즐거움, 놀라움, 분노, 공포, 슬픔]의 감정점수 형식이야.")
	);
	if (IsValid(ChatManager) && HttpManager)
	{
		ChatManager->AddMessageArray(str, Developer);
	}
	//일단 이 developer메시지가 cm에 잘들어가는지만 확인해보자.
	//UE_LOG(LogTemp, Warning, TEXT("%s", ))
}

void ARAIGameMode::BindHM()
{
	if (IsValid(HttpManager))
	{
		HttpManager->ResponseDelegate_NLP.AddDynamic(this, &ARAIGameMode::OnEventDelegate_NLP);
		HttpManager->ResponseDelegate_OpenAI.AddDynamic(this, &ARAIGameMode::OnEventDelegate_OpenAI);
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

void ARAIGameMode::OnEventDelegate_NLP(FString InJsonData)
{
	//언리얼엔진의 멀티쓰레드 환경을 고려해 OnEventDelegate_OpenAI의 CM->AddMessageArray가 작동하는 것을 방지하기 위해 단일쓰레드 강제사용
	AsyncTask(ENamedThreads::GameThread, [this, InJsonData]()
	{
		ScoreString = InJsonData;
		if (ScoreString.IsSet() && ResponseString.IsSet())
		{
			ScoreString.Reset();
			ResponseString.Reset();
			//ChatManager->AddMessageArray();
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
			ScoreString.Reset();
			ResponseString.Reset();
			//ChatManager->AddMessageArray();
		}
	});
}