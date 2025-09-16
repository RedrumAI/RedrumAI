// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAISlideInventoryUI.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "GameMode/RAIPlayerState.h"
#include "GameMode/RAIPlayerController.h"
#include "Animation/WidgetAnimation.h"

void URAISlideInventoryUI::NativeConstruct()
{
	VerticalBox_Button = Cast<UVerticalBox>(GetWidgetFromName(TEXT("VerticalBox_Button")));

	InitSlideInventoryUI();

	ARAIPlayerController* RAIPlayerController = Cast<ARAIPlayerController>(GetOwningPlayer());
	RAIPlayerController->MoveSlideInventoryDelegate.AddDynamic(this, &URAISlideInventoryUI::CallMoveAnimation);
	SlideShowState = false;
}

void URAISlideInventoryUI::InitSlideInventoryUI()
{
	RAIPlayerState = GetOwningPlayerState<ARAIPlayerState>();
	if (IsValid(RAIPlayerState))
	{
		RAIPlayerState->UpdateEvidenceRowsDelegate.AddDynamic(this, &URAISlideInventoryUI::UpdateEvidenceData);

		//InventoryData 크기 초기화
		int32 EvidenceRowLength = RAIPlayerState->GetEvidenceRows().Num();
		InventoryData.SetNum(EvidenceRowLength);

		//SlideInventory 썸네일 초기화
		for (int i = 0;i < VerticalBox_Button->GetChildrenCount();++i)
		{
			UButton* EvidenceButton = Cast<UButton>(VerticalBox_Button->GetChildAt(i));
			UpdateButtonThumbnail(EvidenceButton, EmptyThunmbnail);
		}
	}
	else
	{
		FTimerHandle TimerHandle_InitSlideInventoryUI;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_InitSlideInventoryUI,
			this,
			&URAISlideInventoryUI::InitSlideInventoryUI,
			0.1f,
			false
		);
	}
}

void URAISlideInventoryUI::UpdateEvidenceData()
{
	TArray<FName> EvidenceRows = RAIPlayerState->GetEvidenceRows();

	for (int i = 0;i < EvidenceRows.Num();++i)
	{
		InventoryData[i] = RAIPlayerState->FindEvidenceData(EvidenceRows[i]);

		if (i < VerticalBox_Button->GetChildrenCount())
		{
			if (InventoryData[i])
			{
				UButton* EvidenceButton = Cast<UButton>(VerticalBox_Button->GetChildAt(i));
				UpdateButtonThumbnail(EvidenceButton, InventoryData[i]->EvidenceImage);
			}
			else
			{
				UButton* EvidenceButton = Cast<UButton>(VerticalBox_Button->GetChildAt(i));
				UpdateButtonThumbnail(EvidenceButton, EmptyThunmbnail);
			}
		}
	}
}

void URAISlideInventoryUI::UpdateButtonThumbnail(UButton* InButton, UTexture2D* InThumbnail)
{
	if (!InButton || !InThumbnail)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]:UpdateButtonThumbnail Failed"), *GetName());
		return;
	}
	FButtonStyle NewStyle = InButton->WidgetStyle;

	FSlateBrush NormalBrush;
	NormalBrush.SetResourceObject(InThumbnail);
	NormalBrush.ImageSize = FVector2D(InThumbnail->GetSizeX(), InThumbnail->GetSizeY());

	FSlateBrush HoveredBrush = NormalBrush;
	HoveredBrush.TintColor = FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));

	FSlateBrush PressedBrush = NormalBrush;
	PressedBrush.TintColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.f));

	//EmptyThumbnail 경우 Hoverer, Pressed Tint 원상복귀
	if (InThumbnail == EmptyThunmbnail)
	{
		HoveredBrush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		PressedBrush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}

	NewStyle.SetNormal(NormalBrush);
	NewStyle.SetHovered(HoveredBrush);
	NewStyle.SetPressed(PressedBrush);

	InButton->SetStyle(NewStyle);
}

void URAISlideInventoryUI::CallMoveAnimation()
{
	if (IsPlayingAnimation())
	{
		return;
	}

	if (SlideShowState == true)
	{
		PlayAnimation(SlideRight, 0.0f, 1, EUMGSequencePlayMode::Reverse);
		SlideShowState = false;
	}
	else
	{
		PlayAnimation(SlideRight);
		SlideShowState = true;
	}
}

void URAISlideInventoryUI::OnOpened()
{
}

void URAISlideInventoryUI::OnClosed()
{
}