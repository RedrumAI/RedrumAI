// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIChatLogEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void URAIChatLogEntry::NativeConstruct()
{
	SuspectImage = Cast<UImage>(GetWidgetFromName(TEXT("SuspectImage")));
	UserImage = Cast<UImage>(GetWidgetFromName(TEXT("UserImage")));
	Chat = Cast<UTextBlock>(GetWidgetFromName(TEXT("Chat")));
}

void URAIChatLogEntry::CollapseSuspectImage()
{
	SuspectImage->SetVisibility(ESlateVisibility::Collapsed);
}

void URAIChatLogEntry::CollapseUserImage()
{
	UserImage->SetVisibility(ESlateVisibility::Collapsed);
}

void URAIChatLogEntry::SetChat(FString InChat)
{
	Chat->SetText(FText::FromString(InChat));
}
