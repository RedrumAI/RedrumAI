// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Data/RAIEvidenceData.h"
#include "RAIPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateEvidenceRowsDelegate);

UCLASS()
class REDRUMAI_API ARAIPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "RAI")
	TObjectPtr<UDataTable> EvidenceDataTable;

	TArray<FName> EvidenceRows;

public:
	void AddEvidence(FName EvidenceRowName);
	void RemoveEvidence();

	TArray<FName> GetEvidenceRows();
	FRAIEvidenceData* FindEvidenceData(FName RowName);
	
	UPROPERTY()
	FOnUpdateEvidenceRowsDelegate UpdateEvidenceRowsDelegate;
};
