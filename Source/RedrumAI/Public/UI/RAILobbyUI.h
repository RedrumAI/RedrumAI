// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAILobbyUI.generated.h"

/**
 * 
 */
class UButton;
class UVerticalBox;

UCLASS()
class REDRUMAI_API URAILobbyUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<UButton> Button_Start;
	UPROPERTY()
	TObjectPtr<UButton> Button_Exit;
	UPROPERTY()
	TObjectPtr<UVerticalBox> VerticalBox_Level;

	UPROPERTY()
	TObjectPtr<UButton> Button_Level1;
	UPROPERTY()
	TObjectPtr<UButton> Button_Level2;
	UPROPERTY()
	TObjectPtr<UButton> Button_Level3;

	
public:
	virtual void NativeConstruct() override;

	UFUNCTION() void OnClick_Level1() { SetupLevelByRowName("Level1"); }
	UFUNCTION() void OnClick_Level2() { SetupLevelByRowName("Level2"); }
	UFUNCTION() void OnClick_Level3() { SetupLevelByRowName("Level3"); }

	UFUNCTION()
	void ToggleLevelVerticalBox();
	void SetupLevelByRowName(FName InRowName);

};
