// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIFinalVerdictEntry.h"
#include "UI/RAIFinalVerdictDataObject.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void URAIFinalVerdictEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Image_SuspectImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_SuspectImage")));
	TextBlock_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Name")));;
	Border_Shadow = Cast<UBorder>(GetWidgetFromName(TEXT("Border_Shadow")));

	Border_Shadow->SetVisibility(ESlateVisibility::Hidden);
}

void URAIFinalVerdictEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (URAIFinalVerdictDataObject* DataObject = Cast<URAIFinalVerdictDataObject>(ListItemObject))
	{
		//데이터 가져와서 위젯에 반영
		TextBlock_Name->SetText(FText::FromName(DataObject->GetName()));
		Image_SuspectImage->SetBrushFromTexture(DataObject->GetSuspectImage());
	}
}

void URAIFinalVerdictEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	Border_Shadow->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
