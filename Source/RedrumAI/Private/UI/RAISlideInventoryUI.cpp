// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAISlideInventoryUI.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"

#include "GameMode/RAIGameState.h"

#include "GameMode/RAIPlayerController.h"
#include "Animation/WidgetAnimation.h"
#include "UI/RAIActionList.h"
#include "Components/CanvasPanelSlot.h"
#include "Data/RAIEvidenceData.h"

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
	ARAIGameState* RAIGameState = GetWorld()->GetGameState<ARAIGameState>();
	if (IsValid(RAIGameState) && IsValid(ActionList))
	{
		SetupEvidenceData();

		ActionList->ClickedUseButtonDelegate.AddDynamic(this, &URAISlideInventoryUI::UseEvidence);

		ActionList->ClickedInspectButtonDelegate.AddDynamic(this, &URAISlideInventoryUI::InspectEvidence);
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
	if (ARAIGameState* RAIGameState = GetWorld()->GetGameState<ARAIGameState>())
	{
		RAIGameState->UpdateEvidenceRowsDelegate.AddDynamic(this, &URAISlideInventoryUI::UpdateEvidenceData);

		//InventoryData 크기 초기화
		int32 EvidenceRowLength = RAIGameState->GetEvidenceRows().Num();
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
	ARAIGameState* RAIGameState = GetWorld()->GetGameState<ARAIGameState>();
	TArray<FName> EvidenceRows = RAIGameState->GetEvidenceRows();

	for (int i = 0;i < EvidenceRows.Num();++i)
	{
		if (i < VerticalBox_Button->GetChildrenCount())
		{
			InventoryRowData[i].Key = EvidenceRows[i];
			InventoryRowData[i].Value = nullptr;

			if (InventoryRowData[i].Key == NAME_None)
			{
				UButton* EvidenceButton = Cast<UButton>(VerticalBox_Button->GetChildAt(i));
				UpdateButtonThumbnail(EvidenceButton, EmptyThunmbnail);
			}
			else
			{
				InventoryRowData[i].Value = RAIGameState->FindEvidenceData(EvidenceRows[i]);

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
	FButtonStyle NewStyle = InButton->GetStyle();

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
	UseEvidenceDelegate.Broadcast(ActionText);

	RAIPlayerController->UseEvidence(InventoryRowData[ClickedIndex].Key);

	//소비아이템 처럼 사용 시 아이템 사라지는 기능 추가하려면 GS->RemoveEvidence 호출할 것
}

FText URAISlideInventoryUI::MakeActionText(int32 InIndex)
{
	//MakeActionText
	FString ActionString = FString::Printf(TEXT("['%s'을/를 제출했다.]"), *InventoryRowData[InIndex].Value->DisplayName.ToString());
	FText ActionText = FText::FromString(ActionString);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ActionString);

	return ActionText;
}

void URAISlideInventoryUI::InspectEvidence()
{
	InspectEvidenceDelegate.Broadcast(*InventoryRowData[ClickedIndex].Value);
}