// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIChatLogEntry.generated.h"

class USizeBox;
class UTextBlock;

UCLASS()
class REDRUMAI_API URAIChatLogEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	/* ChatLogEntry 화자 표시용 이미지 폐기로 삭제
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UImage> SuspectImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UImage> UserImage;

	void CollapseSuspectImage();
	void CollapseUserImage();
	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USizeBox> SizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UTextBlock> Chat;

	UFUNCTION(BlueprintCallable)
	void SetChat(FString InChat);
	UFUNCTION(BlueprintCallable)
	void SetChatColor(FSlateColor InColor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float SizeBoxWidth = 800.f;
};
