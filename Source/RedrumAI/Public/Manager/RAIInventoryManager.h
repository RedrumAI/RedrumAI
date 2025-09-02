// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RAIInventoryManager.generated.h"

class ARAIPlayerState;

UCLASS()
class REDRUMAI_API ARAIInventoryManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARAIInventoryManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitInventoryManager();

	TObjectPtr<ARAIPlayerState> RAIPlayerState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	void AddEvidence(FName EvidenceRowName);
};
