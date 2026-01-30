// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIEndingHUD.h"
#include "Components/TextBlock.h"
#include "GameMode/RAIGameState.h"
#include "Animation/WidgetAnimation.h"

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

	if (EndingCreditAnimation)
	{
		// 해당 이벤트 발생 시 내장 함수
		FWidgetAnimationDynamicEvent EndEvent;
		EndEvent.BindDynamic(this, &URAIEndingHUD::WhenAnimationFinished);

		// 끝난 시점 이벤트
		BindToAnimationFinished(EndingCreditAnimation, EndEvent);

		PlayAnimation(EndingCreditAnimation);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EndingCreditAnimation is null. Check BindWidgetAnim name match."));
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

void URAIEndingHUD::WhenAnimationFinished()
{
	bCreditAnimationEnd = true;
	FinishEndingCreditAnimationDelegate.Broadcast();
}
