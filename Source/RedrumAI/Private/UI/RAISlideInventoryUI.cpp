// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAISlideInventoryUI.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "GameMode/RAIPlayerState.h"
#include "GameMode/RAIPlayerController.h"
#include "Animation/WidgetAnimation.h"
#include "UI/RAIActionList.h"
#include "Components/CanvasPanelSlot.h"

void URAISlideInventoryUI::NativeConstruct()
{
	VerticalBox_Button = Cast<UVerticalBox>(GetWidgetFromName(TEXT("VerticalBox_Button")));
	ActionList = Cast<URAIActionList>(GetWidgetFromName(TEXT("WBP_RAIActionList")));

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

		for (int i = 0;i < VerticalBox_Button->GetChildrenCount();++i)
		{
			UButton* EvidenceButton = Cast<UButton>(VerticalBox_Button->GetChildAt(i));

			//SlideInventory 썸네일 초기화
			UpdateButtonThumbnail(EvidenceButton, EmptyThunmbnail);

			//버튼과 버튼클릭함수 연결
			EvidenceButton->OnClicked.AddDynamic(this, &URAISlideInventoryUI::OnEvidenceButtonClicked);
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

void URAISlideInventoryUI::OnEvidenceButtonClicked()
{
	if (SlideShowState == false)
	{
		return;
	}

	// Show ActionList
	ActionList->SetVisibleState(ESlateVisibility::Visible);

	// Move ActionList to CursorPos
	const FVector2D CursorPos = FSlateApplication::Get().GetCursorPos();
	const FGeometry InventoryGeo = GetCachedGeometry();
	const FVector2D LocalPos = InventoryGeo.AbsoluteToLocal(CursorPos);
	if (UCanvasPanelSlot* ActionListSlot = Cast<UCanvasPanelSlot>(ActionList->Slot))
	{
		ActionListSlot->SetPosition(LocalPos);
	}
	ActionList->SetFocus();
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

	ActionList->SetVisibleState(ESlateVisibility::Collapsed);
}

void URAISlideInventoryUI::OnOpened()
{
}

void URAISlideInventoryUI::OnClosed()
{
}

