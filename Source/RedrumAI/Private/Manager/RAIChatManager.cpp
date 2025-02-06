// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RAIChatManager.h"

// Sets default values
ARAIChatManager::ARAIChatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetEmotionScore(EmotionScore, 0, 0, 0, 0, 0, 0);
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

void ARAIChatManager::SetEmotionScore(FEmotionScore& InEmotionScore, float Love, float Joy, float Surprise, float Anger, float Fear, float Sadness)
{
	InEmotionScore.Love = Love;
	InEmotionScore.Joy = Joy;
	InEmotionScore.Surprise = Surprise;
	InEmotionScore.Anger = Anger;
	InEmotionScore.Fear = Fear;
	InEmotionScore.Sadness = Sadness;
}

