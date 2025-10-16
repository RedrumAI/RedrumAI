// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIStageHUDWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/RAIStackWidget.h"
#include "UI/RAIChatUI.h"
#include "UI/RAIChatLogUI.h"
#include "UI/RAIChatLogUIButton.h"
#include "UI/RAISlideInventoryUI.h"

void URAIStageHUDWidget::NativeConstruct()
{
	CanvasPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel")));
	ChatUI = Cast<URAIChatUI>(GetWidgetFromName(TEXT("WBP_RAIChatUI")));
	ChatLogUI = Cast<URAIChatLogUI>(GetWidgetFromName(TEXT("WBP_RAIChatLogUI")));
	ChatLogUIButton = Cast<URAIChatLogUIButton>(GetWidgetFromName(TEXT("WBP_RAIChatLogUIButton")));
	SlideInventoryUI = Cast<URAISlideInventoryUI>(GetWidgetFromName(TEXT("WBP_RAISlideInventoryUI")));

	//ChatUI, LogUI, Button Valid검사. 불통과시 타이머로 다시돌리기
	BindOwningUI();
}

void URAIStageHUDWidget::BindOwningUI()
{
	if (IsValid(ChatUI) && IsValid(ChatLogUI) && IsValid(ChatLogUIButton) && IsValid(SlideInventoryUI))
	{
		//인벤토리Tab의 경우 Open혹은 Close하면 안되기에 제외
		ChatUI->ClickedWidgetDelegate.AddDynamic(this, &URAIStageHUDWidget::OpenUI);
		ChatLogUI->ClickedWidgetDelegate.AddDynamic(this, &URAIStageHUDWidget::OpenUI);
		ChatLogUIButton->RAIButtonClickedDelegate.AddDynamic(this, &URAIStageHUDWidget::ToggleChatLogUI);

		//UI간 바인드 연결
		SlideInventoryUI->SendActionTextDelegate.AddDynamic(ChatUI, &URAIChatUI::SubmitExternalMessage);
	}
	else
	{
		FTimerHandle TimerHandle_BindOwningUI;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_BindOwningUI,
			this,
			&URAIStageHUDWidget::BindOwningUI,
			0.1f,
			false
		);
	}
}

void URAIStageHUDWidget::OpenUI(URAIStackWidget* InUI)
{
	//OldSlot 저장
	UCanvasPanelSlot* OldSlot = Cast<UCanvasPanelSlot>(InUI->Slot);
	check(OldSlot);
	FAnchorData SavedLayout = OldSlot->GetLayout();
	FVector2D SavedAlignment = OldSlot->GetAlignment();
	int32 SavedZOrder = OldSlot->GetZOrder();

	//컴포넌트 구조 최하단으로 이동하여 HUD 맨앞에 표시
	InUI->RemoveFromParent();
	CanvasPanel->AddChild(InUI);

	//SavedData 적용
	UCanvasPanelSlot* NewSlot = Cast<UCanvasPanelSlot>(InUI->Slot);
	check(NewSlot);
	NewSlot->SetLayout(SavedLayout);
	NewSlot->SetAlignment(SavedAlignment);
	NewSlot->SetZOrder(SavedZOrder);

	//UI 표시
	InUI->OnOpened();

	//UIStack 추가
	if (UIStack.Find(InUI) != INDEX_NONE) //기존에 열려있던 UI라면 Stack에서 제거하고 다시 Push
	{
		UIStack.Remove(InUI);
	}
	UIStack.Push(InUI);
}

void URAIStageHUDWidget::CloseUI(URAIStackWidget* InUI)
{
	//UIStack 제거
	if (UIStack.Remove(InUI))
	{
		//제거 성공했다면 제거시 기능 호출
		InUI->OnClosed();
	}
}

void URAIStageHUDWidget::CloseLastUI()
{
	if (UIStack.Num() > 0)
	{
		CloseUI(UIStack[UIStack.Num() - 1]);
	}
	else
	{
		// TODO : ESC메뉴창 추가 예정
		UE_LOG(LogTemp, Warning, TEXT("ESC Menu will be appear.(Now Testing at Q)"));
	}
}

void URAIStageHUDWidget::SwitchChatUI(bool bIsTalking)
{
	if (bIsTalking)
	{
		OpenUI(ChatUI);
	}
	else
	{
		CloseUI(ChatUI);
	}
}

void URAIStageHUDWidget::ToggleChatLogUI()
{
	switch (ChatLogUI->GetVisibility())
	{
	case ESlateVisibility::Visible:
		CloseUI(ChatLogUI);
		break;
	case ESlateVisibility::Hidden:
		//if(메뉴창 visible상태라면) closeUI(메뉴창)
		OpenUI(ChatLogUI);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("[StageHUDWidget] OpenChatLogUI failed, ChatLogUI->GetVisibility is '%s'"),
			*StaticEnum<ESlateVisibility>()->GetNameStringByValue(static_cast<int64>(ChatLogUI->GetVisibility())));
		break;
	}
}

void URAIStageHUDWidget::SetAIChat(FString String)
{
	if (IsValid(ChatUI))
	{
		ChatUI->SetAIChat(String);
	}
}

void URAIStageHUDWidget::AddChatLogUI(FString InRole, FString InMessage)
{
	if (IsValid(ChatLogUI))
	{
		ChatLogUI->CreateChatLogEntry(InRole, InMessage);
	}
}
