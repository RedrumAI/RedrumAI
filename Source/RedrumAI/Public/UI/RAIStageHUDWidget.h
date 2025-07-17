// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIStageHUDWidget.generated.h"

class URAIChatUI;
class URAIChatLogUI;


UCLASS()
class REDRUMAI_API URAIStageHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URAIChatUI> ChatUI;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URAIChatLogUI> ChatLogUI;


	UFUNCTION()	
	void UpdateVisibilityChatUI(ESlateVisibility InState);
	UFUNCTION()
	void UpdateVisiblityChatLogUI(ESlateVisibility InState);

	UFUNCTION()
	void SetAIChat(FString String);
	UFUNCTION()
	void AddChatLogUI(FString InRole, FString InMessage);
};
