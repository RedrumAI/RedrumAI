// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIBaseWidget.h"

FReply URAIBaseWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        ClickedWidgetDelegate.Broadcast(Cast<URAIBaseWidget>(this));
        return FReply::Handled(); // 이벤트 핸들링 완료
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void URAIBaseWidget::OnOpened()
{
    if (GetVisibility() == ESlateVisibility::Hidden)
    {
        //TODO: 끌어올려지는게 아닌 꺼진상태에서 켜진경우
        //1. PlayOpenSound();
    }
    SetVisibility(ESlateVisibility::Visible);
}

void URAIBaseWidget::OnClosed()
{
    if (GetVisibility() == ESlateVisibility::Visible)
    {

    }
    SetVisibility(ESlateVisibility::Hidden);
}
