// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIUIButton.h"
#include "Components/Button.h"

void URAIUIButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Button")));
	
	Button->OnClicked.AddDynamic(this, &URAIUIButton::OnRAIButtonClicked);
}

void URAIUIButton::OnRAIButtonClicked()
{
	RAIButtonClickedDelegate.Broadcast();
}
