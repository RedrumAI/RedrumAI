// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RAIGameInstance.generated.h"

struct FRAIEvidenceData;
struct FRAILevelDataStruct;

UCLASS()
class REDRUMAI_API URAIGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "RAI")
	TObjectPtr<UDataTable> EvidenceDataTable;
	UPROPERTY(EditDefaultsOnly, Category = "RAI")
	TObjectPtr<UDataTable> LevelDataTable;

public:
	UDataTable* GetEvidenceDataTable() { return EvidenceDataTable; }
	UDataTable* GetLevelDataTable() { return LevelDataTable; }

	int32 GetEvidenceDataTableRowNum();
	const FRAIEvidenceData* FindEvidenceData(FName InRowName) const;

	const FRAILevelDataStruct* FindLevelData(FName InRowName) const;
};
