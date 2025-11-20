// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/RAIInteractableActor.h"
#include "RAIConversationInterface.h"
#include "RAISuspect.generated.h"

/**
 * 
 */
UCLASS()
class REDRUMAI_API ARAISuspect : public ARAIInteractableActor, public IRAIConversationInterface
{
	GENERATED_BODY()
	
public:
	ARAISuspect();

protected:
    UPROPERTY(VisibleAnywhere, Category = "RAI")
    TObjectPtr<USceneComponent> FrontAnchor;
    UPROPERTY(VisibleAnywhere, Category = "RAI")
    TObjectPtr<USceneComponent> LeftAnchor;
    UPROPERTY(VisibleAnywhere, Category = "RAI")
    TObjectPtr<USceneComponent> RightAnchor;

public:
	virtual void Interacted(AController* InController) override;

    virtual ERAIConversationSide GetConversationSide(AController* InController) const override;

    virtual FVector GetFrontAnchorLocation() const override;
    virtual FVector GetLeftAnchorLocation()  const override;
    virtual FVector GetRightAnchorLocation() const override;
    virtual FVector GetHeadWorldLocation()   const override;

    //virtual void OnConversationStarted(AController* InController) override;
    //virtual void OnConversationFinished(AController* InController) override;
};
