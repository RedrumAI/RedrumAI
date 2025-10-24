// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/RAIEvidenceData.h"
#include "RAIInspectionUI.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class REDRUMAI_API URAIInspectionUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UImage> MeshImage;
	UPROPERTY()
	TObjectPtr<UTextBlock> DisplayNameTextBlock;
	UPROPERTY()
	TObjectPtr<UTextBlock> DescriptionTextBlock;

public:
	UFUNCTION()
	void UpdateInspectionUI(const FRAIEvidenceData& InEvidenceData);
	void SetDisplayName(FText InText);
	void SetDescription(FText InText);
};
