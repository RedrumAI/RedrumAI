// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAISlideInventoryUI.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "GameMode/RAIPlayerState.h"

void URAISlideInventoryUI::NativeConstruct()
{
	VerticalBox_Button = Cast<UVerticalBox>(GetWidgetFromName(TEXT("VerticalBox_Button")));

	int32 EvidenceDataCount = VerticalBox_Button->GetChildrenCount();
	EvidenceData.SetNum(EvidenceDataCount);

	InitSlideInventoryUI();
}

void URAISlideInventoryUI::InitSlideInventoryUI()
{
	RAIPlayerState = GetOwningPlayerState<ARAIPlayerState>();
	if (IsValid(RAIPlayerState))
	{
		RAIPlayerState->UpdateEvidenceRowsDelegate.AddDynamic(this, &URAISlideInventoryUI::UpdateEvidenceData);
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
		FRAIEvidenceData* RowData = RAIPlayerState->FindEvidenceData(EvidenceRows[i]);
		EvidenceData[i] = RowData;

		if (RowData) //EvidenceRows[i]가 데이터가 있는값이라면(=NAME_None이 아니라면)
		{
			UButton* EvidenceButton = Cast<UButton>(VerticalBox_Button->GetChildAt(i));			
			UpdateButtonThumbnail(EvidenceButton, RowData->EvidenceImage);
		}
		else
		{
			UButton* EvidenceButton = Cast<UButton>(VerticalBox_Button->GetChildAt(i));
			UpdateButtonThumbnail(EvidenceButton, EmptyThunmbnail);
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


void URAISlideInventoryUI::OnOpened()
{
}

void URAISlideInventoryUI::OnClosed()
{
}
