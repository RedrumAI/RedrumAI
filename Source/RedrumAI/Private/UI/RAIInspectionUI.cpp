// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIInspectionUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void URAIInspectionUI::NativeConstruct()
{
	MeshImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_Mesh")));
	DisplayNameTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_DisplayName")));
	DescriptionTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Description")));

	DisplayNameTextBlock->SetAutoWrapText(true);
	DescriptionTextBlock->SetAutoWrapText(true);
}

void URAIInspectionUI::UpdateInspectionUI(const FRAIEvidenceData& InEvidenceData)
{
	//메쉬 업데이트는 InspectionActor에서 알아서.

	SetDisplayName(InEvidenceData.DisplayName);
	SetDescription(InEvidenceData.Description);
}

void URAIInspectionUI::SetDisplayName(FText InText)
{
	DisplayNameTextBlock->SetText(InText);
}

void URAIInspectionUI::SetDescription(FText InText)
{
	DescriptionTextBlock->SetText(InText);
}
