// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIStageHUD.h"
#include "UI/RAIChatUI.h"
#include "UI/RAIChatLogUI.h"

void URAIStageHUD::NativeConstruct()
{
	ChatUI = Cast<URAIChatUI>(GetWidgetFromName(TEXT("WBP_ChatUI")));
	ChatLogUI = Cast<URAIChatLogUI>(GetWidgetFromName(TEXT("WBP_ChatLogUI")));
}

void URAIStageHUD::UpdateVisibilityChatUI(ESlateVisibility InState)
{
	ChatUI->SetVisibility(InState);
}

void URAIStageHUD::UpdateVisiblityChatLogUI(ESlateVisibility InState)
{
	ChatLogUI->SetVisibility(InState);
}

void URAIStageHUD::SetAIChat(FString String)
{
	if (IsValid(ChatUI))
	{
		ChatUI->SetAIChat(String);
	}
}

void URAIStageHUD::AddChatLogUI(FString InRole, FString InMessage)
{
	if (IsValid(ChatLogUI))
	{
		ChatLogUI->CreateChatLogEntry(InRole, InMessage);
	}
}
