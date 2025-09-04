// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAISlideInventoryUI.h"
#include "Components/Button.h"
#include "GameMode/RAIPlayerState.h"

void URAISlideInventoryUI::NativeConstruct()
{
	Button_top = Cast<UButton>(GetWidgetFromName(TEXT("Button_top")));
	Button_middle = Cast<UButton>(GetWidgetFromName(TEXT("Button_middle")));
	Button_bottom = Cast<UButton>(GetWidgetFromName(TEXT("Button_bottom")));

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
	for (auto EachRow : EvidenceRows)
	{
		FRAIEvidenceData* RowData = RAIPlayerState->FindEvidenceData(EachRow);

		//만약 데이터 수정을 한다고하면?
		/*
		FindEvidenceInInventory(이름) = RowData;
		*/

		//TODO: 일단 top부터 비어있으면 들어가는 코드. 그런데 3칸이니까 이렇게쓰지, 확장성을 위한 코드리팩토링이 필요해보인다.
		if (EvidenceData_top)
		{
			EvidenceData_top = RowData;
		}
		else if (EvidenceData_middle)
		{
			EvidenceData_middle = RowData;
		}
		else if (EvidenceData_bottom)
		{
			EvidenceData_bottom = RowData;
		}
	}

	//데이터 수정되었으니 UI에 반영
	UpdateButtonThumbnail();
}

void URAISlideInventoryUI::UpdateButtonThumbnail()
{
	// TODO : 모든 버튼 이미지 업데이트.
	/*
	if(EvidenceData_Top.Thumbnail) //EvidenceData_top이 null값이면 버튼 업데이트 시 터질 수도 있기에 예외처리 해주자.
	{
		ButtonTop.SetImage( EvidenceData_top.Thumbnail )
	}	
	if(EvidenceData_Middle.Thumbnail)
	{
		ButtonMiddle.SetImage( EvidenceData_Middle.Thumbnail ) 
	}
	if(EvidenceData_Bottom.Thumbnail)
	{
		ButtonBottom.SetImage( EvidenceData_Bottom.Thumbnail )
	}
	*/
}

void URAISlideInventoryUI::OnOpened()
{
}

void URAISlideInventoryUI::OnClosed()
{
}
