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
		&ARAIGameMode::InitSettingOpenAI,
		1.0f,
		false
	);

	/*
	FTimerHandle TimerHandle1_tmp;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle1_tmp,
		this,
		&ARAIGameMode::tmpTimerFunction1,
		8.0f,
		false
	);
	*/
}

void ARAIGameMode::InitSettingOpenAI()
{
	FString SettingString;
	SettingString = FString::Printf(
		TEXT("우리는 역할극을 진행 중이야. 나는 형사이고 너는 살인범이야. ")
		TEXT("앞으로 내 메시지에는 너의 감정 상태 점수가 반드시 대괄호로 표시되어 있을 거야. ")
		TEXT("이 감정 점수는 순서대로 [애정, 즐거움, 놀라움, 분노, 공포, 슬픔]을 나타내며, ")
		TEXT("각 값은 0.00부터 0.99까지의 숫자로, 0.99에 가까울수록 해당 감정이 더욱 격해진다는 의미야. ")
		TEXT("하지만 네 응답에서는 절대로 감정 점수를 포함하지 마. ")
		TEXT("즉, 네 모든 응답은 감정 점수를 제거한 순수 텍스트만으로 이루어져야 해. ")
		TEXT("이전 메시지에 감정 점수가 있었다 하더라도 반드시 무시하고, 오직 응답 내용만 보내줘. ")
		TEXT("그리고 나의 첫 번째 시스템 메시지에 대해서는 응답으로 단지 \"!\"만 보내.")
	);
	if (IsValid(ChatManager) && IsValid(HttpManager))
	{
		ChatManager->AddMessageArray(SettingString, Developer);
	}
}

//C++ 테스트를 위한 임시함수. 추후 함수삭제예정
void ARAIGameMode::tmpTimerFunction1()
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

void ARAIGameMode::AskSuspect(const FText& Text)
{
	if (IsValid(ChatManager) && HttpManager)
	{
		ChatManager->AddMessageArray(Text.ToString(), User);
	}
}

void ARAIGameMode::SetScoreStruct(const FString& String)
{
	UE_LOG(LogTemp, Log, TEXT("GM:DeserializeNLP Started"));

	//다시 TArray<TSharedPtr<FJsonVlaue>> 형태로 복구
	TArray<TSharedPtr<FJsonValue>> JsonResponse; //FieldName이 없어서 FJsonValue의 배열을 사용
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(String);

	if (FJsonSerializer::Deserialize(Reader, JsonResponse) && JsonResponse.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("NLP InnerObject :"));
		TArray<TSharedPtr<FJsonValue>> EmotionJson = JsonResponse[0]->AsArray(); // [ [ {},{} ] ] 형태이기에 JsonResponse[0] = 배열

		UE_LOG(LogTemp, Log, TEXT("CM:EmotionScore Love Joy .. start"));
		for (auto EmotionJsonValue : EmotionJson)
		{
			TSharedPtr<FJsonObject> EmotionObject = EmotionJsonValue->AsObject();
			if (!EmotionObject.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("CM: Invalid Emotion JSON Object"));
				continue;
			}

			FString Label;
			double Score = 0;
			if (EmotionObject->TryGetStringField(TEXT("label"), Label) && EmotionObject->TryGetNumberField(TEXT("score"), Score))
			{
				if (Label == TEXT("love")) ScoreStruct->Love = Score;
				else if (Label == TEXT("joy")) ScoreStruct->Joy = Score;
				else if (Label == TEXT("surprise")) ScoreStruct->Surprise = Score;
				else if (Label == TEXT("anger")) ScoreStruct->Anger = Score;
				else if (Label == TEXT("fear")) ScoreStruct->Fear = Score;
				else if (Label == TEXT("sadness")) ScoreStruct->Sadness = Score;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CM: Failed to parse JSON string: %s"), *String);
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
		SetScoreStruct(InJsonData); //단일쓰레드에서 작동하기에 값중 하나만 설정돼도 이 함수가 전부 작동했음을 보장할 수 있다.

		if (ScoreStruct.IsSet() && ResponseString.IsSet())
		{
			ChatManager->AddMessageArray(ScoreStruct.GetValue(), ResponseString.GetValue(), Assistant);
			ScoreStruct.Reset();
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

		if (ScoreStruct.IsSet() && ResponseString.IsSet())
		{
			ChatManager->AddMessageArray(ScoreStruct.GetValue(), ResponseString.GetValue(), Assistant);
			ScoreStruct.Reset();
			ResponseString.Reset();
		}
	});
}

void ARAIGameMode::OnEventDelegate_SendMessageArray(FString MessageString)
{
	HttpManager->SendRequestToOpenAI(MessageString);
}
