// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/RAIMessageRole.h"
#include "Data/RAIEmotionScore.h"
#include "Data/RAILevelDataStruct.h"
#include "RAIGameMode.generated.h"

//AI응답의 문장 혹은 NLP점수가 필요할 경우, 해당 클래스에서 ARAIGameMode::FResponseDelegate, FScoreDelegate에 바인드할것
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseDelegate, FString, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreDelegate, FRAIEmotionScore, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateChatLogUIDelegate, FString, Role, FString, Contents);

class ARAIHttpManager;
class ARAIChatManager;
class ARAIInventoryManager;

enum EUpdateType
{
	Unknown,
	Add,
	Remove,
};

UCLASS()
class REDRUMAI_API ARAIGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<ARAIHttpManager> HttpManager;
	UPROPERTY()
	TObjectPtr<ARAIChatManager> ChatManager;
	UPROPERTY()
	TObjectPtr<ARAIInventoryManager> InventoryManager;

	TOptional<FRAIEmotionScore> ScoreStruct;	//NLP에서 온 점수 FStruct
	TOptional<FString> ResponseString;		//OpenAI에서 온 응답 FString

public:
	UPROPERTY()
	FResponseDelegate SendResponseDelegate;
	UPROPERTY()
	FScoreDelegate SendScoreDelegate;
	UPROPERTY()
	FUpdateChatLogUIDelegate UpdateChatLogUIDelegate;

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

	void UpdateEvidence(FName EvidenceRowName, EUpdateType InType);

	void SetupLevelByRowName(FName InRowName);
};
