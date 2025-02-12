// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/EmotionScore.h"
#include "RAIChatManager.generated.h"

UCLASS()
class REDRUMAI_API ARAIChatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARAIChatManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//추후 AI가 여러개가 된다면 TMap을 사용해 AI이름과 내역을 짝지어 관리하는게 좋겠다.
	TArray<TSharedPtr<FJsonValue>> MessageArray;

	FEmotionScore EmotionScore;

	void SetEmotionScore(TArray<FString> EmotionJson);
};
