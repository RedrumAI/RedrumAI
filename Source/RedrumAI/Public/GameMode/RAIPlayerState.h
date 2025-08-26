// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Data/RAIEvidenceData.h"
#include "RAIPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class REDRUMAI_API ARAIPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	TArray<FRAIEvidenceData> CollectedEvidences;

	//InventoryManager에 구현해야할 기능
	void AddCollectedEvidences();
};
