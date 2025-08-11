// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/RAIMessageRole.h"
#include "Data/RAIEmotionScore.h"
#include "RAIGameMode.generated.h"

//AI응답의 문장이 필요할 경우 FReponseDelegate에, NLP점수가 필요할 경우 FScoreDelegate에 바인드할 것
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseDelegate, FString, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreDelegate, FRAIEmotionScore, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateChatLogUIDelegate, FString, Role, FString, Contents);

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
	UFUNCTION()
	void UpdateChatLogUI();

	void InitSettingOpenAI();
	void tmpTimerFunction1();

	void AskSuspect(const FText Text);

	void SetScoreStruct(const FString& String);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<ARAIHttpManager> HttpManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<ARAIChatManager> ChatManager;

	TOptional<FRAIEmotionScore> ScoreStruct;	//NLP에서 온 점수 FStruct
	TOptional<FString> ResponseString;		//OpenAI에서 온 응답 FString

	UPROPERTY()
	FResponseDelegate SendResponseDelegate;
	UPROPERTY()
	FScoreDelegate SendScoreDelegate;
	UPROPERTY()
	FUpdateChatLogUIDelegate UpdateChatLogUIDelegate;
};
