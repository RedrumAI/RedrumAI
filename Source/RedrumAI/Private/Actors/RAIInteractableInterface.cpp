// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/RAIInteractableInterface.h"

void IRAIInteractableInterface::Interacted()
{
}

void IRAIInteractableInterface::Interacted(AController* InController)
{
}

void IRAIInteractableInterface::EnableHighlight()
{
}

void IRAIInteractableInterface::DisableHighlight()
{
}

void IRAIInteractableInterface::BeginFocused()
{
}

void IRAIInteractableInterface::EndFocused()
{
}

ERAIEvidenceType IRAIInteractableInterface::GetEvidenceType()
{
	return ERAIEvidenceType();
}
