// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RAIStackWidget.h"
#include "RAIFinalVerdictUI.generated.h"


class UTileView;
class UButton;

UCLASS()
class REDRUMAI_API URAIFinalVerdictUI : public URAIStackWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<UTileView> TileView_Suspect;
	UPROPERTY()
	TObjectPtr<UButton> Button_Submit;

public:
	virtual void NativeConstruct() override;
};
