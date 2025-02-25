﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RAIChatManager.h"
#include "Json.h"

// Sets default values
ARAIChatManager::ARAIChatManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARAIChatManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARAIChatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARAIChatManager::SetEmotionScore(FString InJsonData)
{
	UE_LOG(LogTemp, Log, TEXT("CM:SetEmotionScore Started"));

	TArray<TSharedPtr<FJsonValue>> OuterArray;

	//다시 TArray<TSharedPtr<FJsonVlaue>> 형태로 복구
	TArray<TSharedPtr<FJsonValue>> JsonResponse; //FieldName이 없어서 FJsonValue의 배열을 사용해야할 것 같다.
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InJsonData);

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
				if (Label == TEXT("love")) CalculateEmotion(EmotionScore.Love, Score);
				else if (Label == TEXT("joy")) CalculateEmotion(EmotionScore.Joy, Score);
				else if (Label == TEXT("surprise")) CalculateEmotion(EmotionScore.Surprise, Score);
				else if (Label == TEXT("anger")) CalculateEmotion(EmotionScore.Anger, Score);
				else if (Label == TEXT("fear")) CalculateEmotion(EmotionScore.Fear, Score);
				else if (Label == TEXT("sadness")) CalculateEmotion(EmotionScore.Sadness, Score);
				Score = 0;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CM: Failed to parse JSON string: %s"), *InJsonData);
	}

	UE_LOG(LogTemp, Error, TEXT("Love:%f, Joy:%f, Surprise:%f, Anger:%f, Fear:%f, Sadness:%f"),
		EmotionScore.Love, EmotionScore.Joy, EmotionScore.Surprise, EmotionScore.Anger, EmotionScore.Fear, EmotionScore.Sadness);

}

void ARAIChatManager::CalculateEmotion(float& Emotion, float Score)
{
	//한번 질문마다 감정의 사그라짐 = (-0.1) 더함
	Emotion = Emotion - 0.1 + Score;
	//Emotion을 0.XX 형태로 만든다.
	FMath::Clamp(Emotion, 0.00, 0.99); //0.88;
	Emotion = FMath::FloorToFloat(Emotion * 100) / 100; //0.XXXXX -> XX.XXXX -> XX -> 0.XX
}

void ARAIChatManager::AddMessageArray(FString InJsonData, FString Message, EMessageRole MessageRole)
{
	TSharedPtr<FJsonObject> UserMessage = MakeShareable(new FJsonObject);
	switch (MessageRole)
	{
	case EMessageRole::Developer:
	{
		UserMessage->SetStringField("role", "developer");
		UserMessage->SetStringField("content", Message); //Developer문장 추가 경우 InJsonData=NULL;
		break;
	}
	case EMessageRole::User:
	{
		UserMessage->SetStringField("role", "user");

		FString ScoreString = FString::Printf(TEXT("[%f,%f,%f,%f,%f,%f]"),
			EmotionScore.Love, EmotionScore.Joy, EmotionScore.Surprise, EmotionScore.Anger, EmotionScore.Fear, EmotionScore.Sadness);
		FString ScoreAddedMessage = ScoreString.Append(Message);

		UserMessage->SetStringField("content", ScoreAddedMessage);
		break;
	}
	case EMessageRole::Assistant:
	{
		UserMessage->SetStringField("role", "assistant");

		SetEmotionScore(InJsonData);
		FString  ScoreString = FString::Printf(TEXT("[%f,%f,%f,%f,%f,%f]"),
			EmotionScore.Love, EmotionScore.Joy, EmotionScore.Surprise, EmotionScore.Anger, EmotionScore.Fear, EmotionScore.Sadness);
		FString ScoreAddedMessage = ScoreString.Append(Message);
		UserMessage->SetStringField("content", ScoreAddedMessage);
		break;
	}
	default:
		break;
	}	
	MessageArray.Add(MakeShareable(new FJsonValueObject(UserMessage)));
}

void ARAIChatManager::AddMessageArray(FString Message, EMessageRole MessageRole)
{
	TSharedPtr<FJsonObject> UserMessage = MakeShareable(new FJsonObject);
	switch (MessageRole)
	{
	case EMessageRole::Developer:
	{
		UserMessage->SetStringField("role", "developer");
		UserMessage->SetStringField("content", Message); //Developer문장 추가 경우 InJsonData=NULL;
		break;
	}
	case EMessageRole::User:
	{
		UserMessage->SetStringField("role", "user");

		FString ScoreString = FString::Printf(TEXT("[%f,%f,%f,%f,%f,%f]"),
			EmotionScore.Love, EmotionScore.Joy, EmotionScore.Surprise, EmotionScore.Anger, EmotionScore.Fear, EmotionScore.Sadness);
		FString ScoreAddedMessage = ScoreString.Append(Message);

		UserMessage->SetStringField("content", ScoreAddedMessage);
		break;
	}
	case EMessageRole::Assistant:
	{
		UE_LOG(LogTemp, Warning, TEXT("You used Wrong virtaul function with Role::Assistant. Use with InJsonData."));
		break;
	}
	default:
		break;
	}
	MessageArray.Add(MakeShareable(new FJsonValueObject(UserMessage)));

	UE_LOG(LogTemp, Warning, TEXT("AddMessageArray 컴플리트"));
}
