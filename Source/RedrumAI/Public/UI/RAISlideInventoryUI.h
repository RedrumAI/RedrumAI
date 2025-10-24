// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/RAIEvidenceData.h"
#include "RAISlideInventoryUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseEvidenceDelegate, FText, ActionText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInspectEvidenceDelegate, const FRAIEvidenceData&, EvidenceData);

class UButton;
class UVerticalBox;
class ARAIPlayerState;
class UWidgetAnimation;
class URAIActionList;

class ARAIPlayerController;

UCLASS()
class REDRUMAI_API URAISlideInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<UVerticalBox> VerticalBox_Button;

	TObjectPtr<ARAIPlayerState> RAIPlayerState;
	TArray<TPair <FName, FRAIEvidenceData*>> InventoryRowData;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UTexture2D> EmptyThunmbnail;
	UPROPERTY()
	TObjectPtr<URAIActionList> ActionList;

	bool bSlideShowState = false;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> SlideRight; //변수명과 같은 이름의 애니메이션이 자동할당

	TObjectPtr<ARAIPlayerController> RAIPlayerController;
	int32 ClickedIndex = -1;

public:
	UPROPERTY()
	FOnUseEvidenceDelegate UseEvidenceDelegate;
	UPROPERTY()
	FOnInspectEvidenceDelegate InspectEvidenceDelegate;
	
public:
	virtual void NativeConstruct() override;

	void InitSettingSlideInventory();
	void SetupEvidenceData();

	UFUNCTION()
	void UpdateEvidenceData();
	void UpdateButtonThumbnail(UButton* InButton, UTexture2D* InThumbnail);
	UFUNCTION()
	void OnEvidenceButtonClicked();
	
	//값을 찾지못하면 return -1
	int32 FindClickedButtonIndex(); 
	void ShowActionList();
	UFUNCTION()
	void CallMoveAnimation();

	void OnOpened();
	void OnClosed();

	UFUNCTION()
	void UseEvidence();
	FText MakeActionText(int32 InIndex);

	UFUNCTION()
	void InspectEvidence();
};
