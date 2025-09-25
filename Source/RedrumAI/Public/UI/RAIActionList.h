// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RAIActionList.generated.h"

class UVerticalBox;

UCLASS()
class REDRUMAI_API URAIActionList : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<UVerticalBox> VerticalBox_ActionList;

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	void SetVisibleState(ESlateVisibility InState);
};
