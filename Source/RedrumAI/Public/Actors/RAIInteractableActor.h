// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RAIInteractableInterface.h"
#include "RAIInteractableActor.generated.h"


UCLASS()
class REDRUMAI_API ARAIInteractableActor : public AActor, public IRAIInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARAIInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interacted() override;

	virtual void EnableHighlight() override;

	virtual void DisableHighlight() override;

	virtual void BeginFocused() override;

	virtual void EndFocused() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterial> HighlightMaterial;

};
