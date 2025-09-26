// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIStageHUDWidget.generated.h"

class UCanvasPanel;
class URAIStackWidget;
class URAIChatUI;
class URAIChatLogUI;
class URAIChatLogUIButton;
class URAISlideInventoryUI;

UCLASS()
class REDRUMAI_API URAIStageHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<URAIChatUI> ChatUI;
	UPROPERTY()
	TObjectPtr<URAIChatLogUI> ChatLogUI;
	UPROPERTY()
	TObjectPtr<URAIChatLogUIButton> ChatLogUIButton;
	UPROPERTY()
	TObjectPtr<URAISlideInventoryUI> SlideInventoryUI;

	TObjectPtr<UCanvasPanel> CanvasPanel;
	TArray<URAIStackWidget*> UIStack;

	UFUNCTION()
	void BindOwningUI();

	//UI 열면 맨위에 나오게하는 로직
	UFUNCTION()
	void OpenUI(URAIStackWidget* InUI);
	UFUNCTION()
	void CloseUI(URAIStackWidget* InUI);
	//ESC 누르면 맨위 UI삭제하다가 ESC메뉴 표시
	UFUNCTION()
	void CloseLastUI();
	UFUNCTION()
	void ToggleChatLogUI();

	UFUNCTION()
	void SetAIChat(FString String);
	UFUNCTION()
	void AddChatLogUI(FString InRole, FString InMessage);
};
