// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIFinalVerdictUI.h"
#include "Components/TileView.h"
#include "Components/Button.h"
#include "Components/SizeBoxSlot.h"

void URAIFinalVerdictUI::NativeConstruct()
{
	TileView_Suspect = Cast<UTileView>(GetWidgetFromName(TEXT("TileView_Suspect")));
	Button_Submit = Cast<UButton>(GetWidgetFromName(TEXT("Button_Submit")));
	//Button_Submit->OnClicked.AddDynamic(this, &)

	if (USizeBoxSlot* TileViewSlot = Cast<USizeBoxSlot>(TileView_Suspect->Slot))
	{	
		TileViewSlot->SetHorizontalAlignment(HAlign_Fill); //수평은 사이즈박스 크기만큼 제한
		TileViewSlot->SetVerticalAlignment(VAlign_Center); //수직은 사이즈박스의 중앙에 위치하고 최대 사이즈박스 수직만큼 제한
	}

	//TileView_Suspect의 Entry 디자인 설정은 BP에서 적용 (EntrySpacing, Entry Width/Height, TileAlighnment, EntryWidgetClass)

}
