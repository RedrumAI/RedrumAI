// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Data/RAILevelDataStruct.h"
#include "RAIGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishSetLevelDataDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateEvidenceRowsDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggeredDialogueDelegate, int, Idx);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishSetFinalSuspectNameDelegate);


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

	FName FinalSuspectName;

public:
	UPROPERTY()
	FOnFinishSetLevelDataDelegate FinishSetLevelDataDelegate;
	UPROPERTY()
	FOnUpdateEvidenceRowsDelegate UpdateEvidenceRowsDelegate;
	UPROPERTY()
	FOnTriggeredDialogueDelegate TriggeredDialogueDelegate;
	UPROPERTY()
	FOnFinishSetFinalSuspectNameDelegate FinishSetFinalSuspectNameDelegate;
	
public:
	virtual void PostInitializeComponents() override;

	const FRAILevelDataStruct* GetLevelData();
	void SetLevelData(FName InRowName);

	ULevelSequence* GetIntroSequenceAsset();
	
	const TArray<FName> GetEvidenceRows();
	const FRAIEvidenceData* FindEvidenceData(FName RowName) const;

	const FName GetFinalSuspectName();
	void SetFinalSuspectName(FName InSuspectName);

public:
	TArray<FName> GetSuspectNames();
	TArray<UTexture2D*> GetSuspectImages();
	const ULevelSequence* GetEndingSequence();
	FName GetAnswerName();

	void AddEvidence(FName InRowName);
	void RemoveEvidence(FName InRowName);
	
	void TriggerDialogue(int idx);
	FText GetDialogueLineByIndex(int idx);
};
