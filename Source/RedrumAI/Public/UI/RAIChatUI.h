// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RAIBaseWidget.h"
#include "RAIChatUI.generated.h"

class UTextBlock;
class UEditableTextBox;
class UCircularThrobber;
class ARAIPlayerController;


UCLASS()
class REDRUMAI_API URAIChatUI : public URAIBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> AIChat;
	UPROPERTY()
	TObjectPtr<UEditableTextBox> UserChat;
	UPROPERTY()
	TObjectPtr<UCircularThrobber> LoadingCircle;

	TObjectPtr<ARAIPlayerController> RAIPlayerController;

	UFUNCTION()
	void OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod);

	void SetAIChat(FString String);
	void AskSuspect(FText Text);
};
