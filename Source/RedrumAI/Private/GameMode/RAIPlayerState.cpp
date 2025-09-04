// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIPlayerState.h"

void ARAIPlayerState::AddEvidence(FName EvidenceRowName)
{
	FString DebugContext = FString::Printf(TEXT("[%s] : AddEvidence FindRow Called"), *GetName());
	FRAIEvidenceData* FindingData = EvidenceDataTable->FindRow<FRAIEvidenceData>(EvidenceRowName, DebugContext);
	if (FindingData)
	{
		EvidenceRows.Add(EvidenceRowName);
	}

	//TODO : EvidenceRow가 변경되었음을 방송. SlideInventory는 이 때 업데이트
	UpdateEvidenceRowsDelegate.Broadcast();	
}

void ARAIPlayerState::RemoveEvidence()
{


	UpdateEvidenceRowsDelegate.Broadcast();
}


TArray<FName> ARAIPlayerState::GetEvidenceRows()
{
	return EvidenceRows;
}

FRAIEvidenceData* ARAIPlayerState::FindEvidenceData(FName RowName)
{
	FString DebugContext = FString::Printf(TEXT("[%s] : AddEvidence FindRow Called"), *GetName());
	FRAIEvidenceData* FindingData = EvidenceDataTable->FindRow<FRAIEvidenceData>(RowName, DebugContext);
	return FindingData;
}

