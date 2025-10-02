// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/RAIEvidenceData.h"
#include "RAIActionList.generated.h"

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

	FRAIEvidenceData* EvidenceData;

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	void SetEvidenceData(FRAIEvidenceData* InData);

	void SetVisibleState(ESlateVisibility InState);
	UFUNCTION()
	void SwitchUseButton(bool bIsTalking);
	UFUNCTION()
	void OnUseButtonClicked();
	UFUNCTION()
	void OnInspectButtonClicked();
};
