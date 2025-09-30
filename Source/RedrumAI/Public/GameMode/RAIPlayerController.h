// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RAIPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveSlideInventoryDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateTalkingState, bool, bIsTalking);

class URAIStageHUDWidget;
class ARAIGameMode;
class UInputMappingContext;
class UInputAction;

UCLASS()
class REDRUMAI_API ARAIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARAIPlayerController();

protected:
	UPROPERTY()
	TObjectPtr<ARAIGameMode> RAIGameMode;
	UPROPERTY()
	TObjectPtr<URAIStageHUDWidget> StageHUD;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputMappingContext> InputMapping;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_ToggleMouseCursor;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_CloseLastUI;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_MoveSlideInventory;

	bool bIsTalking = false;

public:
	UPROPERTY()
	FMoveSlideInventoryDelegate MoveSlideInventoryDelegate;
	UPROPERTY()
	FOnUpdateTalkingState UpdateTalkingState;

public:
	virtual void BeginPlay() override;
	void BindGM();
	void BindHUD();
	virtual void SetupInputComponent() override;

	void ToggleMouseCursor();
	void CloseLastUI();
	void MoveSlideInventory();
	UFUNCTION()
	void SetAIChat(FString String);
	UFUNCTION()
	void AddChatLogUI(FString InRole, FString InMessage);

	void SetTalkingState(bool InBool);
	UFUNCTION()
	void SwitchTalkingMode(bool InBool);

	void AskSuspect(FText Text);
};
