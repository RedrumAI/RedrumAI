// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Data/RAILevelDataStruct.h"
#include "RAIGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishSetLevelDataDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateEvidenceRowsDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggeredDialogueDelegate, int, Idx);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetFinalSuspect, FName, SelectedSuspectName);


struct FRAIEvidenceData;
class ULevelSequence;

UCLASS()
class REDRUMAI_API ARAIGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	FRAILevelDataStruct LevelData;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TArray<FName> EvidenceRows;

public:
	UPROPERTY()
	FOnFinishSetLevelDataDelegate FinishSetLevelDataDelegate;
	UPROPERTY()
	FOnUpdateEvidenceRowsDelegate UpdateEvidenceRowsDelegate;
	UPROPERTY()
	FOnTriggeredDialogueDelegate TriggeredDialogueDelegate;

public:
	virtual void PostInitializeComponents() override;
	const FRAILevelDataStruct* GetLevelData();
	ULevelSequence* GetIntroSequenceAsset();
	void SetLevelData(FName InRowName);
	
	TArray<FName> GetSuspectNames();
	TArray<UTexture2D*> GetSuspectImages();
	ULevelSequence* GetEndingSequence(FName InSuspectName);
	FName GetAnswerName();

	void AddEvidence(FName InRowName);
	void RemoveEvidence(FName InRowName);
	
	TArray<FName> GetEvidenceRows() const;
	const FRAIEvidenceData* FindEvidenceData(FName RowName) const;

	void TriggerDialogue(int idx);
	//void SetFinalSuspect(FName SelectedSuspectName);
};
