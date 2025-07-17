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
	TObjectPtr<URAIStageHUDWidget> StageHUD;
	UPROPERTY()
	TObjectPtr<ARAIGameMode> RAIGameMode;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Test;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Test2;

	UFUNCTION()
	void TestFunc1();
	UFUNCTION()
	void TestFunc2();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void BindGM();

	UFUNCTION()
	void SetAIChat(FString String);

	UFUNCTION()
	void AddChatLogUI(FString InRole, FString InMessage);
	
	void AskSuspect(FText Text);

};
