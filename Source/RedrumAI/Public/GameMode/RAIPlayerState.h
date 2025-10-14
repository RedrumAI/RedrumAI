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
	virtual void PostInitializeComponents() override;

	void AddEvidence(FName InRowName);
	void RemoveEvidence(FName InRowName);

	TArray<FName> GetEvidenceRows() const;
	FRAIEvidenceData* FindEvidenceData(FName RowName) const;
	
	UPROPERTY()
	FOnUpdateEvidenceRowsDelegate UpdateEvidenceRowsDelegate;
};
