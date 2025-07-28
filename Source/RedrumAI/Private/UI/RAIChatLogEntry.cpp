// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIChatLogEntry.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void URAIChatLogEntry::NativeConstruct()
{
	Super::NativeConstruct();

	SizeBox = Cast<USizeBox>(GetWidgetFromName(TEXT("SizeBox")));
	Chat = Cast<UTextBlock>(GetWidgetFromName(TEXT("Chat")));

	SizeBox->SetWidthOverride(SizeBoxWidth);
}

void URAIChatLogEntry::SetChat(FString InChat)
{
	Chat->SetText(FText::FromString(InChat));
}

void URAIChatLogEntry::SetChatColor(FSlateColor InColor)
{
	Chat->SetColorAndOpacity(InColor);

		//FColor HexColor = FColor::FromHex(TEXT("a40606"));
		//FLinearColor MyColor = HexColor.ReinterpretAsLinear();
		//FSlateColor RedColor(MyColor);

		//FSlateColor RedColor(FLinearColor(0.643f, 0.024f, 0.024f, 1.0f));
		//FSlateColor BlueColor(FLinearColor(0.f, 0.f, 0.f, 1.f));
}
