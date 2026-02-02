// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIEndingHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishEndingCreditAnimationDelegate);

class UTextBlock;
class ARAIGameState;
class UWidgetAnimation;

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
	
	// BP에서 만든 애니메이션과 "이름이 같아야" 자동 바인딩됨
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> EndingCreditAnimation;
	
public:
	UPROPERTY()
	FOnFinishEndingCreditAnimationDelegate FinishEndingCreditAnimationDelegate;

	bool bCreditAnimationEnd = false;

public:
	virtual void NativeConstruct() override;
	
	//허드 가시성 토글 ShowHUD(bool InVisibility)
	void ShowHUD(bool InVisibility);
	void PlayEndingCreditAnimation();

	UFUNCTION()
	void SetLineText(int idx);

	UFUNCTION()
	void NoticeEndingCreditAniamtionFinished();
}; 
