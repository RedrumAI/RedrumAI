// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RAIInteractableInterface.h"
#include "RAIInteractableActor.generated.h"

class UBoxComponent;

UCLASS()
class REDRUMAI_API ARAIInteractableActor : public AActor, public IRAIInteractableInterface
{
	GENERATED_BODY()
	
public:	
	ARAIInteractableActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UMaterial> HighlightMaterial;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UBoxComponent> CollisionBox;
	
	UPROPERTY(EditAnywhere)
	ERAIEvidenceType EvidenceType;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interacted() override;
	virtual void Interacted(AController* InController) override;

	//캐릭터 Focus시, 관련 함수
	virtual void BeginFocused() override;
	virtual void EndFocused() override;
	virtual void EnableHighlight() override;
	virtual void DisableHighlight() override;

	virtual ERAIEvidenceType GetEvidenceType() override;
};
