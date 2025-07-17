// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIChatLogUIButton.h"
#include "Components/Button.h"

void URAIChatLogUIButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Button")));

	/* 버튼 사이즈 일괄조정
	Button->WidgetStyle.Normal.SetImageSize(FVector2D(ButtonSizeX, ButtonSizeY));
	Button->WidgetStyle.Hovered.SetImageSize(FVector2D(ButtonSizeX, ButtonSizeY));
	Button->WidgetStyle.Pressed.SetImageSize(FVector2D(ButtonSizeX, ButtonSizeY));
	*/
}
