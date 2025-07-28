// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RAIBaseWidget.h"
#include "Data/MessageRole.h"
#include "RAIChatLogUI.generated.h"

class UScrollBox;
class URAIChatLogEntry;

UCLASS()
class REDRUMAI_API URAIChatLogUI : public URAIBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UScrollBox> ScrollBox;

	//생성할 ChatLongEntry BP버전 삽입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<URAIChatLogEntry> ChatLogEntryBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FSlateColor EntryRedColor = FLinearColor(1.f, 0.f, 0.f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FSlateColor EntryBlueColor = FLinearColor(0.f, 0.f, 1.f, 1.f);

	void CreateChatLogEntry(FString InRole, FString InMessage);
	void SetChatLogEntrySetting(TObjectPtr<URAIChatLogEntry> InChatLogEntry, FString InRole, FString InMessage);
};
