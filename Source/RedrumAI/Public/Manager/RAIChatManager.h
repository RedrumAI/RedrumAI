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

	//���� AI�� �������� �ȴٸ� TMap�� ����� AI�̸��� ������ ¦���� �����ϴ°� ���ڴ�.
	TArray<TSharedPtr<FJsonValue>> MessageArray;

	FEmotionScore EmotionScore;

	void SetEmotionScore(TArray<FString> EmotionJson);
};
