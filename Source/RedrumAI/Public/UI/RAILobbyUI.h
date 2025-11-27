// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAILobbyUI.generated.h"

/**
 * 
 */
class UButton;

UCLASS()
class REDRUMAI_API URAILobbyUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<UButton> Button_Start;
	UPROPERTY()
	TObjectPtr<UButton> Button_Exit;
	
public:
	virtual void NativeConstruct() override;

};
