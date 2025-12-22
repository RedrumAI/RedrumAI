// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIGameInstance.h"
#include "Data/RAIEvidenceData.h"
#include "Data/RAILevelDataStruct.h"

int32 URAIGameInstance::GetEvidenceDataTableRowNum()
{
	return EvidenceDataTable->GetRowMap().Num();
}

const FRAIEvidenceData* URAIGameInstance::FindEvidenceData(FName InRowName) const
{
	FString DebugContext = FString::Printf(TEXT("[%s] : FindEvidenceData Called"), *GetName());
	FRAIEvidenceData* FindingData = EvidenceDataTable->FindRow<FRAIEvidenceData>(InRowName, DebugContext);
	
	return FindingData;
}

const FRAILevelDataStruct* URAIGameInstance::FindLevelData(FName InRowName) const
{
	FString DebugContext = FString::Printf(TEXT("[%s] : FindLevelData Called"), *GetName());
	FRAILevelDataStruct* FindingData = LevelDataTable->FindRow<FRAILevelDataStruct>(InRowName, DebugContext);

	return FindingData;
}
