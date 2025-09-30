// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIActionList.h"

void URAIActionList::NativeConstruct()
{
	SetIsFocusable(true);
}

void URAIActionList::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	SetVisibleState(ESlateVisibility::Collapsed);
}

void URAIActionList::SetVisibleState(ESlateVisibility InState)
{
	SetVisibility(InState);
}
