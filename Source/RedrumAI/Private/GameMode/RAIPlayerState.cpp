// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/RAIPlayerState.h"

void ARAIPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//BP 생성용 EvidenceRowsCount 임시지정
	int32 EvidenceRowsCount = 0;
	if (EvidenceDataTable)
	{
		EvidenceRowsCount = EvidenceDataTable->GetRowMap().Num();
	}
	EvidenceRows.SetNum(EvidenceRowsCount);
	
	//초기화하지 않아도 0이지만, 명시용
	for (int i = 0; i < EvidenceRows.Num(); ++i)
	{
		EvidenceRows[i] = NAME_None;	
	}
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
	//TODO::인자로 받은 이름을 EvidenceRows에서 찾아 NAME_None으로 바꿔주기.

	UpdateEvidenceRowsDelegate.Broadcast();
}

TArray<FName> ARAIPlayerState::GetEvidenceRows() const
{
	return EvidenceRows;
}

FRAIEvidenceData* ARAIPlayerState::FindEvidenceData(FName RowName) const
{
	FString DebugContext = FString::Printf(TEXT("[%s] : AddEvidence FindRow Called"), *GetName());
	FRAIEvidenceData* FindingData = EvidenceDataTable->FindRow<FRAIEvidenceData>(RowName, DebugContext);
	return FindingData;
}

