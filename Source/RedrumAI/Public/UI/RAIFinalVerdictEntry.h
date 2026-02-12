// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "RAIFinalVerdictEntry.generated.h"

class UTextBlock;
class UImage;
class UBorder;
 

UCLASS()
class REDRUMAI_API URAIFinalVerdictEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<UImage> Image_SuspectImage;
	UPROPERTY()
	TObjectPtr<UTextBlock> TextBlock_Name;
	UPROPERTY()
	TObjectPtr<UBorder> Border_Shadow;
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;


};
