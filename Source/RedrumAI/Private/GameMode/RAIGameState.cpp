// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIGameState.h"
#include "GameMode/RAIGameInstance.h"
#include "Data/RAIEvidenceData.h"

void ARAIGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	URAIGameInstance* RAIGameInstance = GetWorld()->GetGameInstance<URAIGameInstance>();
	EvidenceRows.SetNum(RAIGameInstance->GetEvidenceDataTableRowNum());

	//초기화하지 않아도 0이지만, 명시용
	for (int i = 0; i < EvidenceRows.Num(); ++i)
	{
		EvidenceRows[i] = NAME_None;
	}
}

void ARAIGameState::AddEvidence(FName InRowName)
{
	if (const FRAIEvidenceData* FindingData = FindEvidenceData(InRowName))
	{
		for (int i = 0; i < EvidenceRows.Num(); ++i)
		{
			if (EvidenceRows[i] == NAME_None)
			{
				EvidenceRows[i] = InRowName;
				break;
			}
		}
	}

	UpdateEvidenceRowsDelegate.Broadcast();
}

void ARAIGameState::RemoveEvidence(FName InRowName)
{
	for (int i = 0; i < EvidenceRows.Num(); ++i)
	{
		if (EvidenceRows[i] == InRowName)
		{
			EvidenceRows[i] = NAME_None;
			break;
		}
	}

	UpdateEvidenceRowsDelegate.Broadcast();
}

TArray<FName> ARAIGameState::GetEvidenceRows() const
{
	return EvidenceRows;
}

const FRAIEvidenceData* ARAIGameState::FindEvidenceData(FName RowName) const
{
	URAIGameInstance* RAIGameInstance = GetWorld()->GetGameInstance<URAIGameInstance>();
	return RAIGameInstance->FindEvidenceData(RowName);
}