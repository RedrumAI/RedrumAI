// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIGameInstance.h"
#include "Data/RAIEvidenceData.h"

int32 URAIGameInstance::GetEvidenceDataTableRowNum()
{
	return EvidenceDataTable->GetRowMap().Num();
}

const FRAIEvidenceData* URAIGameInstance::FindEvidenceData(FName RowName) const
{
	FString DebugContext = FString::Printf(TEXT("[%s] : FindRow Called"), *GetName());
	FRAIEvidenceData* FindingData = EvidenceDataTable->FindRow<FRAIEvidenceData>(RowName, DebugContext);
	
	return FindingData;
}
