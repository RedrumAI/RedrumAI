// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"	// FHttpRequestPtr����� �����̹Ƿ� ����߰�(���X)
#include "Interfaces/IHttpResponse.h"
#include "RAIHttpManager.generated.h"

//TArray<TSharedPtr<FJsonValue>>�� TSharedPtr�� Delegate���� ����� �� ���⿡ FString���� ��ȯ�Ͽ� ���� ���
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNLPResponseDelegate, TArray<FString>, InJsonData); 

UCLASS(Blueprintable)
class REDRUMAI_API ARAIHttpManager : public AActor
{
	GENERATED_BODY()
	
	public:	

	ARAIHttpManager();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SendRequestToOpenAI(const FString& InputText);

	void OnOpenAIResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SendRequestToNLP(const FString& InputText);

	void OnNLPResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	FString APIKey_OpenAI;
	FString URL_OpenAI = TEXT("https://api.openai.com/v1/chat/completions"); // chat model�� ���
	FString APIKey_NLP;
	FString URL_NLP = TEXT("https://api-inference.huggingface.co/models/bhadresh-savani/distilbert-base-uncased-emotion");	//TEXT��ũ�θ� ����ϸ� �߰� TCHAR��ȯ �ʿ�X
					
	UPROPERTY()
	FOnNLPResponseDelegate ResponseDelegate_NLP;

	int cnt = 0;
};
