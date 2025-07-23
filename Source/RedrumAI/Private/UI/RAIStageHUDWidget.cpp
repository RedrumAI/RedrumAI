// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIStageHUDWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/RAIChatUI.h"
#include "UI/RAIChatLogUI.h"

void URAIStageHUDWidget::NativeConstruct()
{
	CanvasPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel")));
	ChatUI = Cast<URAIChatUI>(GetWidgetFromName(TEXT("WBP_ChatUI")));
	ChatLogUI = Cast<URAIChatLogUI>(GetWidgetFromName(TEXT("WBP_ChatLogUI")));

}

void URAIStageHUDWidget::OnEventDelegate_LogUIButton()
{
	if (ChatLogUI->GetVisibility() == ESlateVisibility::Visible)
	{
		ChatLogUI->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		ChatLogUI->SetVisibility(ESlateVisibility::Visible);
	}
}

void URAIStageHUDWidget::UpdateVisibilityChatUI(ESlateVisibility InState)
{
	ChatUI->SetVisibility(InState);
}

void URAIStageHUDWidget::UpdateVisiblityChatLogUI(ESlateVisibility InState)
{
	ChatLogUI->SetVisibility(InState);
}

void URAIStageHUDWidget::OpenUI(UUserWidget* InUI)
{
	//OldSlot 저장
	FAnchorData SavedLayout;
	FVector2D SavedAlignment;
	int32 SavedZOrder;
	if (UCanvasPanelSlot* OldSlot = Cast<UCanvasPanelSlot>(InUI->Slot))
	{
		SavedLayout = OldSlot->GetLayout();
		SavedAlignment = OldSlot->GetAlignment();
		SavedZOrder = OldSlot->GetZOrder();
	}	

	//컴포넌트 구조 최하단으로 이동하여 HUD 맨앞에 표시
	InUI->RemoveFromParent();
	CanvasPanel->AddChild(InUI);

	//SavedData 적용
	if (UCanvasPanelSlot* NewSlot = Cast<UCanvasPanelSlot>(InUI->Slot))
	{
		NewSlot->SetLayout(SavedLayout);
		NewSlot->SetAlignment(SavedAlignment);
		NewSlot->SetZOrder(SavedZOrder);
	}

	//UI 표시
	InUI->SetVisibility(ESlateVisibility::Visible);

	//UIStack 추가
	if (UIStack.Find(InUI) != INDEX_NONE) //기존에 열려있던 UI라면 Stack에서 제거하고 다시 Push
	{
		UIStack.Remove(InUI);
	}	
	UIStack.Push(InUI);
	
}

void URAIStageHUDWidget::CloseUI(UUserWidget* InUI)
{
	//UI 숨김 (Collapse의 경우 Slot정보가 지워질까 염려하여 Hidden으로 사용)
	InUI->SetVisibility(ESlateVisibility::Hidden);

	//UIStack 추가
	UIStack.Remove(InUI);
}

void URAIStageHUDWidget::CloseLastUI()
{
	if (UIStack.Num() > 0)
	{
		CloseUI(UIStack[UIStack.Num() - 1]);
	}
	else
	{
		// TODO : ESC메뉴창 출력
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
