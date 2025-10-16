// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIStackWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedWidget, URAIStackWidget*, ClickedWidget);

UCLASS()
class REDRUMAI_API URAIStackWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	FOnClickedWidget ClickedWidgetDelegate;

	//가제: WhenOpened(), 해당UI가 OpenUI 되었을 때, 실행해야 하는 기능함수 (ex. InventoryWidget::DoAnimation)
	virtual void OnOpened();
	virtual void OnClosed();
};
