// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIChatLogUI.h"
#include "UI/RAIChatLogEntry.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"

void URAIChatLogUI::NativeConstruct()
{
	ScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox")));
}


void URAIChatLogUI::CreateChatLogEntry(FString InRole, FString InMessage)
{
	/*UClass* WidgetClass = StageHUDClassPath.TryLoadClass<URAIStageHUDWidget>();
	StageHUD = CreateWidget<URAIStageHUDWidget>(this, WidgetClass);*/

	URAIChatLogEntry* CreatedLogEntry = CreateWidget<URAIChatLogEntry>(this, ChatLogEntryBP);

	ScrollBox->AddChild(CreatedLogEntry);
	SetChatLogEntrySetting(CreatedLogEntry, InRole, InMessage);

	ScrollBox->ScrollToEnd();
}

void URAIChatLogUI::SetChatLogEntrySetting(TObjectPtr<URAIChatLogEntry> InChatLogEntry, FString InRole, FString InMessage)
{
	if (InRole == TEXT("developer"))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetChatLogEntrySetting Worked!!!"));
		InChatLogEntry->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (InRole == TEXT("user"))
	{
		UScrollBoxSlot* EntryAsSlot = Cast<UScrollBoxSlot>(InChatLogEntry->Slot);
		EntryAsSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);

		InChatLogEntry->SetChat(InMessage);
		InChatLogEntry->SetChatColor(EntryBlueColor);
	}
	else if (InRole == TEXT("assistant"))
	{
		UScrollBoxSlot* EntryAsSlot = Cast<UScrollBoxSlot>(InChatLogEntry->Slot);
		EntryAsSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
		InChatLogEntry->SetChat(InMessage);
		InChatLogEntry->SetChatColor(EntryRedColor);
	}
}
