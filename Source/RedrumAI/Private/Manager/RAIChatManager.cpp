// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RAIChatManager.h"

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

void ARAIChatManager::SetEmotionScore(TArray<TSharedPtr<FJsonValue>> EmotionJson)
{
	for (auto EmotionJsonValue : EmotionJson)
	{
		TSharedPtr<FJsonObject> EmotionObject = EmotionJsonValue->AsObject();
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
