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

	InitSettingSlideInventory();

	RAIPlayerController = Cast<ARAIPlayerController>(GetOwningPlayer());
	RAIPlayerController->MoveSlideInventoryDelegate.AddDynamic(this, &URAISlideInventoryUI::CallMoveAnimation);
	bSlideShowState = false;
}

void URAISlideInventoryUI::InitSettingSlideInventory()
{
	RAIPlayerState = GetOwningPlayerState<ARAIPlayerState>();
	if (IsValid(RAIPlayerState) && IsValid(ActionList))
	{
		SetupEvidenceData();

		ActionList->UseButtonClickedDelegate.AddDynamic(this, &URAISlideInventoryUI::UseEvidence);
	}
	else
	{
		FTimerHandle TimerHandle_InitSettingSlideInventory;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_InitSettingSlideInventory,
			this,
			&URAISlideInventoryUI::InitSettingSlideInventory,
			0.1f,
			false
		);
	}
}

void URAISlideInventoryUI::SetupEvidenceData()
{
	if (IsValid(RAIPlayerState))
	{
		RAIPlayerState->UpdateEvidenceRowsDelegate.AddDynamic(this, &URAISlideInventoryUI::UpdateEvidenceData);

		//InventoryData 크기 초기화
		int32 EvidenceRowLength = RAIPlayerState->GetEvidenceRows().Num();
		InventoryRowData.SetNum(EvidenceRowLength);

		for (int i = 0;i < VerticalBox_Button->GetChildrenCount();++i)
		{
			UButton* EvidenceButton = Cast<UButton>(VerticalBox_Button->GetChildAt(i));

			//SlideInventory 썸네일 초기화
			UpdateButtonThumbnail(EvidenceButton, EmptyThunmbnail);

			//버튼과 버튼클릭함수 연결
			EvidenceButton->OnClicked.AddDynamic(this, &URAISlideInventoryUI::OnEvidenceButtonClicked);
		}
	}
}

void URAISlideInventoryUI::UpdateEvidenceData()
{
	TArray<FName> EvidenceRows = RAIPlayerState->GetEvidenceRows();

	for (int i = 0;i < EvidenceRows.Num();++i)
	{
		InventoryRowData[i].Key = EvidenceRows[i];
		InventoryRowData[i].Value = RAIPlayerState->FindEvidenceData(EvidenceRows[i]);

		if (i < VerticalBox_Button->GetChildrenCount())
		{
			if (InventoryRowData[i].Key == NAME_None)
			{
				UButton* EvidenceButton = Cast<UButton>(VerticalBox_Button->GetChildAt(i));
				UpdateButtonThumbnail(EvidenceButton, EmptyThunmbnail);
			}
			else
			{
				UButton* EvidenceButton = Cast<UButton>(VerticalBox_Button->GetChildAt(i));
				UpdateButtonThumbnail(EvidenceButton, InventoryRowData[i].Value->EvidenceImage);
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
	if (bSlideShowState == false)
	{
		return;
	}

	ClickedIndex = FindClickedButtonIndex();
	if ( ClickedIndex != -1 && InventoryRowData[ClickedIndex].Key!=NAME_None)
	{
		ActionList->SetEvidenceData(InventoryRowData[ClickedIndex].Value);
		ShowActionList();
	}
}

int32 URAISlideInventoryUI::FindClickedButtonIndex()
{
	for (int32 i = 0;i < VerticalBox_Button->GetChildrenCount();++i)
	{
		UButton* Button = Cast<UButton>(VerticalBox_Button->GetChildAt(i));
		if(Button->HasUserFocus(GetOwningPlayer()))
		{
			return i;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[SlideInventoryUI] : FindClickedButtonIndex Failed"));
	return -1;
}

void URAISlideInventoryUI::ShowActionList()
{
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

	if (bSlideShowState == true)
	{
		PlayAnimation(SlideRight, 0.0f, 1, EUMGSequencePlayMode::Reverse);
		bSlideShowState = false;
	}
	else
	{
		PlayAnimation(SlideRight);
		bSlideShowState = true;
	}

	ActionList->SetVisibleState(ESlateVisibility::Collapsed);
}

void URAISlideInventoryUI::OnOpened()
{
}

void URAISlideInventoryUI::OnClosed()
{
}


void URAISlideInventoryUI::UseEvidence()
{
	FText ActionText = MakeActionText(ClickedIndex);
	SendActionTextDelegate.Broadcast(ActionText);

	//누구를 보내야하나?
	//RAIPlayerState->RemoveEvidence(InventoryRowData[ClickedIndex].Key);
	//RAIPlayerController->GM의 RemoveEvidence부르는 함수만들어서 거기다가 요청.
	RAIPlayerController->UseEvidence(InventoryRowData[ClickedIndex].Key);



}

FText URAISlideInventoryUI::MakeActionText(int32 InIndex)
{
	//MakeActionText
	FString ActionString = FString::Printf(TEXT("['%s'을/를 제출했다.]"), *InventoryRowData[InIndex].Value->DisplayName.ToString());
	FText ActionText = FText::FromString(ActionString);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ActionString);

	return ActionText;
}

