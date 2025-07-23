// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIBaseWidget.h"

FReply URAIBaseWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        ClickedWidgetDelegate.Broadcast(Cast<UUserWidget>(this));
        return FReply::Handled(); // 이벤트 핸들링 완료
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
