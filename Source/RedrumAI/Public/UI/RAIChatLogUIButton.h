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
	
public:
	virtual void NativeConstruct() override;

	/* BP에 실시간 적용불가(NativeConstruct는 Runtime적용이기 때문에)하여 폐기
	//버튼 이미지 사이즈 일괄조정, Tint및 Image는 BP에서 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customize")
	float ButtonSizeX = 64.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customize")
	float ButtonSizeY = 64.f;
	*/

	UFUNCTION()
	void OnRAIButtonClicked();

	UPROPERTY()
	TObjectPtr<UButton> Button;
	UPROPERTY()
	FOnRAIButtonClicked RAIButtonClickedDelegate;

};
