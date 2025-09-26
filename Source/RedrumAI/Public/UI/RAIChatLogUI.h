// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RAIStackWidget.h"
#include "Data/RAIMessageRole.h"
#include "RAIChatLogUI.generated.h"

class UScrollBox;
class URAIChatLogEntry;

UCLASS()
class REDRUMAI_API URAIChatLogUI : public URAIStackWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<UScrollBox> ScrollBox;

	//생성할 ChatLongEntry BP버전 삽입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAI")
	TSubclassOf<URAIChatLogEntry> ChatLogEntryBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAI")
	FSlateColor EntryChatSuspectColor = FLinearColor(0.8f, 0.05f, 0.05f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAI")
	FSlateColor EntryChatPlayerColor = FLinearColor(0.4f, 0.8f, 1.f, 1.f);

	void CreateChatLogEntry(FString InRole, FString InMessage);
	void SetChatLogEntrySetting(TObjectPtr<URAIChatLogEntry> InChatLogEntry, FString InRole, FString InMessage);
};
