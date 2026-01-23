// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIEndingHUD.h"
#include "Components/TextBlock.h"
#include "GameMode/RAIGameState.h"

void URAIEndingHUD::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_Line = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Line")));
	TextBlock_Skip = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Skip")));

	RAIGameState = GetWorld()->GetGameState<ARAIGameState>();
	if (RAIGameState)
	{
		RAIGameState->TriggeredDialogueDelegate.AddDynamic(this, &URAIEndingHUD::SetLineText);
	}
}

void URAIEndingHUD::SetLineText(int idx)
{
	if (RAIGameState)
	{
		FText CurrentLine = RAIGameState->GetDialogueLineByIndex(idx);
		TextBlock_Line->SetText(CurrentLine);
	}
}
