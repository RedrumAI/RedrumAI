// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIChatLogEntry.generated.h"

class USizeBox;
class UTextBlock;

UCLASS()
class REDRUMAI_API URAIChatLogEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	UPROPERTY()
	TObjectPtr<UTextBlock> Chat;

	UFUNCTION(BlueprintCallable)
	void SetChat(FString InChat);
	UFUNCTION(BlueprintCallable)
	void SetChatColor(FSlateColor InColor);
};
