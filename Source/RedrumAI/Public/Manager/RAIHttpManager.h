// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"	// FHttpRequestPtr사용을 위함이므로 헤더추가(상속X)
#include "Interfaces/IHttpResponse.h"

#include "RAIHttpManager.generated.h"

UCLASS(Blueprintable)
class REDRUMAI_API URAIHttpManager : public UObject
{
	GENERATED_BODY()
	
public:

	URAIHttpManager();

	UFUNCTION(BlueprintCallable)
	void SendRequestToOpenAI(const FString& Prompt);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


public:
	FString OpenAI_API_Key;
};
