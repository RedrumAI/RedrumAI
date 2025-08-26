// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAISlideInventoryUI.h"
#include "Components/Button.h"

void URAISlideInventoryUI::NativeConstruct()
{
	Button_top = Cast<UButton>(GetWidgetFromName(TEXT("Button_top")));
	Button_middle = Cast<UButton>(GetWidgetFromName(TEXT("Button_middle")));
	Button_bottom = Cast<UButton>(GetWidgetFromName(TEXT("Button_bottom")));

	//Button->OnClicked.AddDynamic(this, &URAIChatLogUIButton::OnRAIButtonClicked);
}

void URAISlideInventoryUI::OnOpened()
{
}

void URAISlideInventoryUI::OnClosed()
{
}
