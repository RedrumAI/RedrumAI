// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RAIPlayerController.generated.h"

class URAIStageHUD;
class URAIChatUI;
class URAIChatLogUI;
class ARAIGameMode;

UCLASS()
class REDRUMAI_API ARAIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARAIPlayerController();

	TObjectPtr<URAIStageHUD> StageHUD;
	TObjectPtr<URAIChatUI> ChatUI;
	TObjectPtr<URAIChatLogUI>ChatLogUI;

	UPROPERTY()
	TObjectPtr<ARAIGameMode> RAIGameMode;

	virtual void BeginPlay();

	void BindGM();

	UFUNCTION()
	void AddAIChat(FString String);

	UFUNCTION()
	void AddChatLogUI(FString InRole, FString InMessage);
	
	void AskSuspect(FText Text);
};
