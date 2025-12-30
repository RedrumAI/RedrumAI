// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIFinalVerdictUI.h"
#include "Components/TileView.h"
#include "Components/Button.h"
#include "Components/SizeBoxSlot.h"

#include "Data/RAIFinalVerdictDataStruct.h"
#include "UI/RAIFinalVerdictDataObject.h"
#include "GameMode/RAIGameState.h"

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

	//TileView_Suspect의 Entry 디자인 설정은 BP에서 적용. (EntrySpacing, Entry Width/Height, TileAlighnment, EntryWidgetClass)

	TileView_Suspect->OnItemClicked().AddUObject(this, &URAIFinalVerdictUI::OnSuspectTileViewItemClicked);
	Button_Submit->OnClicked.AddDynamic(this, &URAIFinalVerdictUI::OnSubmitButtonClicked);

	if (ARAIGameState* RAIGameState = GetWorld()->GetGameState<ARAIGameState>())
	{
		RAIGameState->FinishSetLevelDataDelegate.AddDynamic(this, &URAIFinalVerdictUI::BuildVerdictEntries);
	}
}

void URAIFinalVerdictUI::BuildVerdictEntries()
{
	TileView_Suspect->ClearListItems();

	//Get TileView Object Data from GameState
	ARAIGameState* RAIGameState = GetWorld()->GetGameState<ARAIGameState>();
	TArray<FName> SuspectNames = RAIGameState->GetSuspectNames();
	TArray<UTexture2D*> SuspectImages = RAIGameState->GetSuspectImages();
	FName AnswerName = RAIGameState->GetAnswerName();

	for (int i = 0; i < SuspectNames.Num();++i)
	{
		URAIFinalVerdictDataObject* DataObject = NewObject<URAIFinalVerdictDataObject>(this);
		DataObject->SetName(SuspectNames[i]);
		DataObject->SetSuspectImage(SuspectImages[i]);
		DataObject->SetIsAnswer(AnswerName == SuspectNames[i] ? true : false);

		TileView_Suspect->AddItem(DataObject);
	}
}

void URAIFinalVerdictUI::OnSuspectTileViewItemClicked(UObject* ClickedItem)
{
	// 같은 아이템을 다시 클릭 -> 토글 해제
	if (LastClickedItem == ClickedItem && TileView_Suspect->GetSelectedItem() == ClickedItem)
	{
		TileView_Suspect->ClearSelection();
		LastClickedItem = nullptr;
		return;
	}

	// 다른 아이템 클릭 -> 선택
	TileView_Suspect->SetSelectedItem(ClickedItem);
	LastClickedItem = ClickedItem;
}

void URAIFinalVerdictUI::OnSubmitButtonClicked()
{
	// UObejct* SelectedSuspect = TileView_Suspect->GetSelectedItem();
	// SelectedItem이 존재한다면 suspect name 을 컨트롤러로 전달
	// gamemode 정답 판정 및 엔딩 시퀀스
}
