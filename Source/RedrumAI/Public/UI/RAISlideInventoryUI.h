// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/RAIEvidenceData.h"
#include "RAISlideInventoryUI.generated.h"

class UButton;
class UVerticalBox;
class ARAIPlayerState;

UCLASS()
class REDRUMAI_API URAISlideInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<UVerticalBox> VerticalBox_Button;

	TArray<FRAIEvidenceData*> InventoryData;

	UPROPERTY(EditAnywhere, Category = "RAI")
	UTexture2D* EmptyThunmbnail;

	TObjectPtr<ARAIPlayerState> RAIPlayerState;

public:
	virtual void NativeConstruct() override;

	void InitSlideInventoryUI();
	UFUNCTION()
	void UpdateEvidenceData();
	void UpdateButtonThumbnail(UButton* InButton, UTexture2D* InThumbnail);

	void OnOpened();
	void OnClosed();
};
