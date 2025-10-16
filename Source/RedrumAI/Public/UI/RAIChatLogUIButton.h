// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIChatLogUIButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRAIButtonClicked);

class UButton;

UCLASS()
class REDRUMAI_API URAIChatLogUIButton : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<UButton> Button;
	
public:
	UPROPERTY()
	FOnRAIButtonClicked RAIButtonClickedDelegate;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRAIButtonClicked();
};
