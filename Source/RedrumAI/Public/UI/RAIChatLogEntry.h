// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIChatLogEntry.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class REDRUMAI_API URAIChatLogEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UImage> SuspectImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UImage> UserImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UTextBlock> Chat;

	void CollapseSuspectImage();
	void CollapseUserImage();
	void SetChat(FString InChat);
};
