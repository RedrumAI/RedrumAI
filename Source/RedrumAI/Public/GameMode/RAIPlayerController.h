// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RAIPlayerController.generated.h"

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

	UPROPERTY()
	TObjectPtr<ARAIGameMode> RAIGameMode;

	virtual void BeginPlay() override;
	void BindGM();
	void AskSuspect(FText Text);


	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputMappingContext> InputMapping;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_ToggleMouseCursor;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_CloseLastUI;

	virtual void SetupInputComponent() override;
	UFUNCTION()
	void ToggleMouseCursor();
	UFUNCTION()
	void CloseLastUI();


	UPROPERTY()
	TObjectPtr<URAIStageHUDWidget> StageHUD;
	UFUNCTION()
	void SetAIChat(FString String);
	UFUNCTION()
	void AddChatLogUI(FString InRole, FString InMessage);	

};
