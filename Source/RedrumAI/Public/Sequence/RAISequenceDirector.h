// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceDirector.h"
#include "RAISequenceDirector.generated.h"

class ARAIGameState;
class URAIEndingHUD;

UCLASS()
class REDRUMAI_API URAISequenceDirector : public ULevelSequenceDirector
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<ARAIGameState> GameState;

	//TObjectPtr<URAIEndingHUD> EndingHUD;

public:
	UFUNCTION(BlueprintCallable, Category = "RAI")
	void Initialize();

	UFUNCTION(BlueprintCallable, Category = "RAI")
	void TriggerDialogue(int idx);

	UFUNCTION(BlueprintCallable, Category = "RAI")
	void CheckEndingHold();

	UFUNCTION()
	void ReleaseEndingHold();
};
