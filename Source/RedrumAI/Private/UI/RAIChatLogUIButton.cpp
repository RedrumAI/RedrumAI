// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIChatLogUIButton.h"
#include "Components/Button.h"

void URAIChatLogUIButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Button")));
	
	Button->OnClicked.AddDynamic(this, &URAIChatLogUIButton::OnRAIButtonClicked);
}

void URAIChatLogUIButton::OnRAIButtonClicked()
{
	RAIButtonClickedDelegate.Broadcast();
}
