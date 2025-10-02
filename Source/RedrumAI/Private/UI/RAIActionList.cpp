// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIActionList.h"
#include "Components/Button.h"
#include "GameMode/RAIPlayerController.h"

void URAIActionList::NativeConstruct()
{
	SetIsFocusable(true);

	Button_Use = Cast<UButton>(GetWidgetFromName(TEXT("Button_Use")));
	Button_Inspect = Cast<UButton>(GetWidgetFromName(TEXT("Button_Inspect")));

	Button_Use->OnClicked.AddDynamic(this, &URAIActionList::OnUseButtonClicked);
	Button_Inspect->OnClicked.AddDynamic(this, &URAIActionList::OnInspectButtonClicked);

	ARAIPlayerController* RAIPlayerController = GetOwningPlayer<ARAIPlayerController>();
	RAIPlayerController->UpdateTalkingStateDelegate.AddDynamic(this, &URAIActionList::SwitchUseButton);

	Button_Use->SetIsEnabled(RAIPlayerController->GetTalkingState());

}

void URAIActionList::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	SetVisibleState(ESlateVisibility::Collapsed);
}

void URAIActionList::SetEvidenceData(FRAIEvidenceData* InData)
{
	EvidenceData = InData;
}

void URAIActionList::SetVisibleState(ESlateVisibility InState)
{
	SetVisibility(InState);
}

void URAIActionList::SwitchUseButton(bool bIsTalking)
{
	Button_Use->SetIsEnabled(bIsTalking);
}

void URAIActionList::OnUseButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("UseButton Used"));
	
	//TODO: 제출하는 기능 필요

	//GM->AskSuspect에 지정된 텍스트를 보내야함. 대화문장과 다른 '행동을 의미하는 텍스트'로 전송한다
	//ex: ['오염된 장갑'을 제출했다.]
	/*
	FString 행동문장 = Printf(TEXT("['%s' 을/를 제출했다.]"), ActionList->EvidenceData->DisplayName));
	GM->AskSuspect(행동문장)
	*/
	
	FString ActionString = FString::Printf(TEXT("['%s'을/를 제출했다.]"), *EvidenceData->DisplayName.ToString());
	FText ActionText = FText::FromString(ActionString);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ActionString);

	ARAIPlayerController* RAIPlayerController = GetOwningPlayer<ARAIPlayerController>();
	RAIPlayerController->AskSuspect(ActionText);

}

void URAIActionList::OnInspectButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("InspectButton Used"));
}
