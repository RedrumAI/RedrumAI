// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/RAIEvidenceData.h"
#include "RAIInspectionUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateActorMeshDelegate, UStaticMesh*, InMesh);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRotateActorDelegate, float, DeltaX, float, DeltaY);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoomActorDelegate, float, Wheel);

class UTextBlock;
class UImage;
class ARAIPlayerController;

UCLASS()
class REDRUMAI_API URAIInspectionUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> DisplayNameTextBlock;
	UPROPERTY()
	TObjectPtr<UTextBlock> DescriptionTextBlock;

	virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry&, const FPointerEvent&) override;
	virtual FReply NativeOnMouseMove(const FGeometry&, const FPointerEvent&) override;
	virtual FReply NativeOnMouseWheel(const FGeometry&, const FPointerEvent&) override;

	

	TWeakObjectPtr<ARAIPlayerController> RAIPlayerController;

	bool bDragging = false;

public:
	UPROPERTY()
	FOnUpdateActorMeshDelegate UpdateActorMeshDelegate;
	UPROPERTY()
	FOnRotateActorDelegate RotateActorDelegate;
	UPROPERTY()
	FOnZoomActorDelegate ZoomActorDelegate;

public:
	void BindInspectionActor();

	UFUNCTION()
	void OpenInspectionUI(const FRAIEvidenceData& InEvidenceData);
	UFUNCTION()
	void CloseInspectionUI();

	void UpdateInspectionUI(const FRAIEvidenceData& InEvidenceData);
	void SetDisplayName(FText InText);
	void SetDescription(FText InText);
};
