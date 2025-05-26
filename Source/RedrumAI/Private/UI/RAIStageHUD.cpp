// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIStageHUD.h"
#include "UI/RAIChatUI.h"
#include "UI/RAIChatLogUI.h"

void URAIStageHUD::NativeConstruct()
{
	ChatUI = Cast<URAIChatUI>(GetWidgetFromName(TEXT("WBP_ChatUI")));
	ChatLogUI = Cast<URAIChatLogUI>(GetWidgetFromName(TEXT("WBP_ChatLogUI")));
}
