// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RAIInventoryManager.generated.h"

class ARAIGameState;

UCLASS()
class REDRUMAI_API ARAIInventoryManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ARAIInventoryManager();

	void AddEvidence(FName EvidenceRowName);
	void RemoveEvidence(FName EvidenceRowName);
};
