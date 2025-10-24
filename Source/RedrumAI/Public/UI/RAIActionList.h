// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/RAIEvidenceData.h"
#include "RAIActionList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedUseButtonDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedInspectButtonDelegate);

class UVerticalBox;
class UButton;

UCLASS()
class REDRUMAI_API URAIActionList : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<UVerticalBox> VerticalBox_ActionList;
	UPROPERTY()
	TObjectPtr<UButton> Button_Use;
	UPROPERTY()
	TObjectPtr<UButton> Button_Inspect;

public:
	UPROPERTY()
	FOnClickedUseButtonDelegate ClickedUseButtonDelegate;
	UPROPERTY()
	FOnClickedInspectButtonDelegate ClickedInspectButtonDelegate;

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	void SetVisibleState(ESlateVisibility InState);
	UFUNCTION()
	void SwitchUseButton(bool bIsTalking);
	UFUNCTION()
	void OnUseButtonClicked();
	UFUNCTION()
	void OnInspectButtonClicked();
};
