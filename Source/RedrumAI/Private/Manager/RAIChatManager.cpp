// Fill out your copyright notice in the Description page of Project Settings.


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

void ARAIChatManager::SetEmotionScore(const FEmotionScore& InEmotionStruct)
{
	CalculateEmotion(EmotionScore.Anger, InEmotionStruct.Anger);
	CalculateEmotion(EmotionScore.Fear, InEmotionStruct.Fear);
	CalculateEmotion(EmotionScore.Joy, InEmotionStruct.Joy);
	CalculateEmotion(EmotionScore.Love, InEmotionStruct.Love);
	CalculateEmotion(EmotionScore.Sadness, InEmotionStruct.Sadness);
	CalculateEmotion(EmotionScore.Surprise, InEmotionStruct.Surprise);
}

void ARAIChatManager::CalculateEmotion(float& Emotion, float Score)
{
	//한번 질문마다 감정의 사그라짐 = (-0.1) 더함
	Emotion = Emotion - 0.1f + Score;
	//Emotion을 0.XX 형태로 만든다.
	Emotion = FMath::Clamp(Emotion, 0.00f, 0.99f); //0.88;
	Emotion = FMath::FloorToFloat(Emotion * 100) / 100; //0.XXXXX -> XX.XXXX -> XX -> 0.XX
}

void ARAIChatManager::AddMessageArray(const FEmotionScore& EmotionStruct, FString Message, EMessageRole MessageRole)
{
	TSharedPtr<FJsonObject> UserMessage = MakeShareable(new FJsonObject);
	switch (MessageRole)
	{
	case EMessageRole::Developer:
	{
		UE_LOG(LogTemp, Warning, TEXT("You used Wrong virtaul function with Role::Developer. Use without InJsonData."));
		break;
	}
	case EMessageRole::User:
	{
		UE_LOG(LogTemp, Warning, TEXT("You used Wrong virtaul function with Role::User. Use without InJsonData."));
		break;
	}
	case EMessageRole::Assistant:
	{
		UserMessage->SetStringField("role", "assistant");

		if (MessageArray.Num() > 2) //첫 developer와 assistant("!")는 점수계산 제외
		{

			//구조체가 들어오니 해당 구조체에서 점수뽑아서 기존점수에 계산하기
			SetEmotionScore(EmotionStruct);
			FString  ScoreString = FString::Printf(TEXT("[%.2f,%.2f,%.2f,%.2f,%.2f,%.2f]"),
				EmotionScore.Love, EmotionScore.Joy, EmotionScore.Surprise, EmotionScore.Anger, EmotionScore.Fear, EmotionScore.Sadness);
			FString ScoreAddedMessage = ScoreString.Append(Message);
			UserMessage->SetStringField("content", ScoreAddedMessage);
		}
		else
		{
			UserMessage->SetStringField("content", Message);
		}
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
		UserMessage->SetStringField("role", "developer"); //24년말부턴 system이 아닌 developer
		UserMessage->SetStringField("content", Message); //Developer문장 추가 경우 InJsonData=NULL;
		break;
	}
	case EMessageRole::User:
	{
		UserMessage->SetStringField("role", "user");

		FString ScoreString = FString::Printf(TEXT("[%.2f,%.2f,%.2f,%.2f,%.2f,%.2f]"),
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
	
	SendMessageArrayToGM();
}

void ARAIChatManager::SendMessageArrayToGM()
{
	//MessageArray를 Fstring으로 변환
	FString MessageString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&MessageString);
	FJsonSerializer::Serialize(MessageArray, Writer);

	SendMessageDelegate.Broadcast(MessageString);
}

void ARAIChatManager::ShowMessageArray()
{
	//MessageArray내 FJsonObject를 전부 출력
	UE_LOG(LogTemp, Warning, TEXT("<로그 출력 시작>"));
	for (auto MessageOne : MessageArray)
	{
		FString MessageContent = MessageOne->AsObject()->GetStringField(TEXT("content"));

		UE_LOG(LogTemp, Log, TEXT("%s"), *MessageContent);
	}
	UE_LOG(LogTemp, Warning, TEXT("<로그 출력 완료>"));
}

const TArray<TPair<FString, FString>> ARAIChatManager::GetChatLog()
{
	TArray<TPair<FString, FString>> ChatLogArray;

	for (auto MessageOne : MessageArray)
	{
		FString MessageRole = MessageOne->AsObject()->GetStringField(TEXT("role"));
		FString MessageContent = MessageOne->AsObject()->GetStringField(TEXT("content"));

		ChatLogArray.Add(TPair<FString, FString>(MessageRole, MessageContent));
	}

	return ChatLogArray;
}



