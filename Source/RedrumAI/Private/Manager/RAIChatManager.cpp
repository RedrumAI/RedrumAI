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
				if (Label == TEXT("love")) EmotionScore.Love = Score;
				else if (Label == TEXT("joy")) EmotionScore.Joy = Score;
				else if (Label == TEXT("surprise")) EmotionScore.Surprise = Score;
				else if (Label == TEXT("anger")) EmotionScore.Anger = Score;
				else if (Label == TEXT("fear")) EmotionScore.Fear = Score;
				else if (Label == TEXT("sadness")) EmotionScore.Sadness = Score;
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