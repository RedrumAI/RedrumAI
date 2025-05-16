// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIChatLogUI.generated.h"

class UScrollBox;

UCLASS()
class REDRUMAI_API URAIChatLogUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UScrollBox> ScrollBox;

	void CreateChatLogEntry();
};
