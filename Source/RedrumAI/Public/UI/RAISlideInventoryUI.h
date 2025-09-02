// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/RAIEvidenceData.h"
#include "RAISlideInventoryUI.generated.h"

class UButton;

UCLASS()
class REDRUMAI_API URAISlideInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UButton> Button_top;
	UPROPERTY()
	TObjectPtr<UButton> Button_middle;
	UPROPERTY()
	TObjectPtr<UButton> Button_bottom;

	FRAIEvidenceData* EvidenceData_top;
	FRAIEvidenceData* EvidenceData_middle;
	FRAIEvidenceData* EvidenceData_bottom;

public:
	void OnOpened();
	void OnClosed();
};
