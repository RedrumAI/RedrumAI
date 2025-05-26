// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIStageHUD.generated.h"

class URAIChatUI;
class URAIChatLogUI;


UCLASS()
class REDRUMAI_API URAIStageHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URAIChatUI> ChatUI;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URAIChatLogUI> ChatLogUI;

};
