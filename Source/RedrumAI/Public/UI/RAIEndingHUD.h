// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIEndingHUD.generated.h"

class ARAIGameState;
class UTextBlock;

UCLASS()
class REDRUMAI_API URAIEndingHUD : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TObjectPtr<UTextBlock> TextBlock_Line;
	UPROPERTY()
	TObjectPtr<UTextBlock> TextBlock_Skip;

	TObjectPtr<ARAIGameState> RAIGameState;

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void SetLineText(int idx);
};
