// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/RAIInteractableActor.h"
#include "RAISuspect.generated.h"

/**
 * 
 */
UCLASS()
class REDRUMAI_API ARAISuspect : public ARAIInteractableActor
{
	GENERATED_BODY()
	
public:
	ARAISuspect();

protected:

	
public:
	virtual void Interacted(AController* InController) override;
};
