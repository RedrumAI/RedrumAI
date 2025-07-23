// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIStageHUDWidget.generated.h"

class URAIChatUI;
class URAIChatLogUI;
class UCanvasPanel;

UCLASS()
class REDRUMAI_API URAIStageHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URAIChatUI> ChatUI;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URAIChatLogUI> ChatLogUI;

	TObjectPtr<UCanvasPanel> CanvasPanel;
	TArray<UUserWidget*> UIStack;

	UFUNCTION()
	void OnEventDelegate_LogUIButton();
	UFUNCTION()	
	void UpdateVisibilityChatUI(ESlateVisibility InState);
	UFUNCTION()
	void UpdateVisiblityChatLogUI(ESlateVisibility InState);

	//UI 열면 맨위에 나오게하는 로직
	UFUNCTION()
	void OpenUI(UUserWidget* InUI);
	UFUNCTION()
	void CloseUI(UUserWidget* InUI);
	//ESC 누르면 맨위 UI삭제하다가 ESC메뉴 표시
	UFUNCTION()
	void CloseLastUI();

	UFUNCTION()
	void SetAIChat(FString String);
	UFUNCTION()
	void AddChatLogUI(FString InRole, FString InMessage);
};
