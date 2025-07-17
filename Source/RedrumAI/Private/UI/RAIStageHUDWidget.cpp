// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIStageHUDWidget.h"
#include "UI/RAIChatUI.h"
#include "UI/RAIChatLogUI.h"

void URAIStageHUDWidget::NativeConstruct()
{
	ChatUI = Cast<URAIChatUI>(GetWidgetFromName(TEXT("WBP_ChatUI")));
	ChatLogUI = Cast<URAIChatLogUI>(GetWidgetFromName(TEXT("WBP_ChatLogUI")));
}

void URAIStageHUDWidget::UpdateVisibilityChatUI(ESlateVisibility InState)
{
	ChatUI->SetVisibility(InState);
}

void URAIStageHUDWidget::UpdateVisiblityChatLogUI(ESlateVisibility InState)
{
	ChatLogUI->SetVisibility(InState);
}

void URAIStageHUDWidget::SetAIChat(FString String)
{
	if (IsValid(ChatUI))
	{
		ChatUI->SetAIChat(String);
	}
}

void URAIStageHUDWidget::AddChatLogUI(FString InRole, FString InMessage)
{
	if (IsValid(ChatLogUI))
	{
		ChatLogUI->CreateChatLogEntry(InRole, InMessage);
	}
}
