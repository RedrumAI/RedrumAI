// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIChatUI.generated.h"

class UTextBlock;
class UEditableTextBox;
class UCircularThrobber;
class ARAIPlayerController;


UCLASS()
class REDRUMAI_API URAIChatUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UTextBlock> AIChat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEditableTextBox> UserChat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCircularThrobber> LoadingCircle;

	TObjectPtr<ARAIPlayerController> RAIPlayerController;

	UFUNCTION()
	void OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod);

	void SetAIChat(FString String);
	void AskSuspect(FText Text);
};
