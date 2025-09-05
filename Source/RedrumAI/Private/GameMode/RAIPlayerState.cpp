// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIPlayerState.h"

ARAIPlayerState::ARAIPlayerState()
{
	int32 EvidenceRowsCount = EvidenceDataTable->GetRowMap().Num();
	EvidenceRows.SetNum(EvidenceRowsCount);
}

void ARAIPlayerState::AddEvidence(FName EvidenceRowName)
{
	FString DebugContext = FString::Printf(TEXT("[%s] : AddEvidence FindRow Called"), *GetName());
	FRAIEvidenceData* FindingData = EvidenceDataTable->FindRow<FRAIEvidenceData>(EvidenceRowName, DebugContext);
	if (FindingData)
	{
		for (int i = 0; i < EvidenceRows.Num(); ++i)
		{
			if (EvidenceRows[i] == NAME_None)
			{
				EvidenceRows[i] = EvidenceRowName;
				break;
			}
		}
	}

	UpdateEvidenceRowsDelegate.Broadcast();	
}

void ARAIPlayerState::RemoveEvidence()
{


	UpdateEvidenceRowsDelegate.Broadcast();
}

TArray<FName> ARAIPlayerState::GetEvidenceRows() const
{
	return EvidenceRows;
}

FRAIEvidenceData* ARAIPlayerState::FindEvidenceData(FName RowName)
{
	FString DebugContext = FString::Printf(TEXT("[%s] : AddEvidence FindRow Called"), *GetName());
	FRAIEvidenceData* FindingData = EvidenceDataTable->FindRow<FRAIEvidenceData>(RowName, DebugContext);
	return FindingData;
}

