// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/MessageRole.h"
#include "Data/EmotionScore.h"
#include "RAIGameMode.generated.h"

class ARAIHttpManager;
class ARAIChatManager;

UCLASS()
class REDRUMAI_API ARAIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARAIGameMode();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void BindHM();
	UFUNCTION()
	void BindCM();

	UFUNCTION()
	void OnEventDelegate_NLP(FString InJsonData);
	UFUNCTION()
	void OnEventDelegate_OpenAI(FString Message);
	UFUNCTION()
	void OnEventDelegate_SendMessageArray(FString MessageString);

	void InitSettingOpenAI();
	void tmpTimerFunction1();

	void AskSuspect(const FText& Text);

	void SetScoreStruct(const FString& String);



	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<ARAIHttpManager> HttpManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<ARAIChatManager> ChatManager;

	TOptional<FEmotionScore> ScoreStruct;	//NLP에서 온 점수 FStruct
	TOptional<FString> ResponseString;		//OpenAI에서 온 응답 FString
};
