// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RAIStackWidget.h"
#include "RAIChatUI.generated.h"

class UTextBlock;
class UEditableTextBox;
class UImage;
class ARAIPlayerController;


UCLASS()
class REDRUMAI_API URAIChatUI : public URAIStackWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> AIChat;
	UPROPERTY()
	TObjectPtr<UEditableTextBox> UserChat;
	UPROPERTY()
	TObjectPtr<UImage> LoadingImage;

	TObjectPtr<ARAIPlayerController> RAIPlayerController;

	UFUNCTION()
	void OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod);

	void SetAIChat(FString String);
	void AskSuspect(FText Text);

	virtual void OnClosed() override;
};
