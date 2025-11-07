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
	SetVisibleState(ESlateVisibility::Collapsed);

	ClickedUseButtonDelegate.Broadcast();
}

void URAIActionList::OnInspectButtonClicked()
{
	SetVisibleState(ESlateVisibility::Collapsed);

	ClickedInspectButtonDelegate.Broadcast();
}
