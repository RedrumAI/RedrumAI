// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIGameState.h"
#include "GameMode/RAIGameInstance.h"
#include "Data/RAIEvidenceData.h"
#include "Data/RAILevelDataStruct.h"
#include "LevelSequence.h"
#include "Data/RAIFinalVerdictDataStruct.h"
#include "Data/RAICutsceneDialogueDataAsset.h"

void ARAIGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (URAIGameInstance* RAIGameInstance = GetWorld()->GetGameInstance<URAIGameInstance>())
	{
		EvidenceRows.SetNum(RAIGameInstance->GetEvidenceDataTableRowNum());
	}

	//초기화
	for (int i = 0; i < EvidenceRows.Num(); ++i)
	{
		EvidenceRows[i] = NAME_None;
	}
}

const FRAILevelDataStruct* ARAIGameState::GetLevelData()
{
	return &LevelData;
}

ULevelSequence* ARAIGameState::GetIntroSequenceAsset()
{
	return LevelData.IntroSequenceAsset;
}

void ARAIGameState::SetupStageState(FName InLevelRowName)
{
	ClearEvidenceRow();

	FinalSuspectName = NAME_None;

	SetLevelData(InLevelRowName);

	FinishSetupStageStateDelegate.Broadcast();
}

void ARAIGameState::ClearEvidenceRow()
{
	for (int i = 0; i < EvidenceRows.Num(); ++i)
	{
		EvidenceRows[i] = NAME_None;
	}
}

void ARAIGameState::SetLevelData(FName InRowName)
{
	URAIGameInstance* RAIGameInstance = GetWorld()->GetGameInstance<URAIGameInstance>();

	const FRAILevelDataStruct* FoundData = RAIGameInstance->FindLevelData(InRowName);

	if (!FoundData)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelData not Found"));
		return;
	}
	
	LevelData = *FoundData;
}

const FName ARAIGameState::GetFinalSuspectName()
{
	return FinalSuspectName;
}

void ARAIGameState::SetFinalSuspectName(FName InSuspectName)
{
	FinalSuspectName = InSuspectName;
	FinishSetFinalSuspectNameDelegate.Broadcast();
}

TArray<FName> ARAIGameState::GetSuspectNames()
{
	TArray<FName> SuspectNames;

	for (auto LevelSuspect : LevelData.LevelSuspects)
	{
		FDataTableRowHandle SuspectRow = LevelSuspect.Suspect;

		FRAIFinalVerdictDataStruct* SuspectData = SuspectRow.DataTable->FindRow<FRAIFinalVerdictDataStruct>(
			SuspectRow.RowName,
			TEXT("Suspect"),//디버그표시용 이름: 찾으려는 핸들
			true
		);

		SuspectNames.Add(SuspectData->Name);
	}

	return SuspectNames;
}

TArray<UTexture2D*> ARAIGameState::GetSuspectImages()
{
	TArray<UTexture2D*> SuspectImages;

	for (auto LevelSuspect : LevelData.LevelSuspects)
	{
		FDataTableRowHandle SuspectRow = LevelSuspect.Suspect;

		FRAIFinalVerdictDataStruct* SuspectData = SuspectRow.DataTable->FindRow<FRAIFinalVerdictDataStruct>(
			SuspectRow.RowName,
			TEXT("Suspect"),//디버그표시용 이름: 찾으려는 핸들
			true
		);

		SuspectImages.Add(SuspectData->SuspectImage);
	}

	return SuspectImages;
}

const ULevelSequence* ARAIGameState::GetEndingSequence()
{
	for (auto LevelSuspect : LevelData.LevelSuspects)
	{
		if (LevelSuspect.Suspect.RowName == FinalSuspectName)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *LevelSuspect.Suspect.RowName.ToString());

			return LevelSuspect.EndingSequence;
		}
	}

	return nullptr;
}

FName ARAIGameState::GetAnswerName()
{
	FRAIFinalVerdictDataStruct* AnswerData = LevelData.Answer.DataTable->FindRow<FRAIFinalVerdictDataStruct>(
		LevelData.Answer.RowName,
		TEXT("Answer"), //디버그표시용 이름: 찾으려는 핸들
		true
	);

	return AnswerData->Name;
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

const TArray<FName> ARAIGameState::GetEvidenceRows()
{
	return EvidenceRows;
}

const FRAIEvidenceData* ARAIGameState::FindEvidenceData(FName RowName) const
{
	URAIGameInstance* RAIGameInstance = GetWorld()->GetGameInstance<URAIGameInstance>();
	return RAIGameInstance->FindEvidenceData(RowName);
}

void ARAIGameState::TriggerDialogue(int idx)
{
	TriggeredDialogueDelegate.Broadcast(idx);
}

FText ARAIGameState::GetDialogueLineByIndex(int idx)
{
	for (auto LevelSuspect : LevelData.LevelSuspects)
	{
		if (LevelSuspect.Suspect.RowName == FinalSuspectName)
		{
			if (LevelSuspect.CutsceneDialogue->Lines.IsValidIndex(idx))
			{
				return LevelSuspect.CutsceneDialogue->Lines[idx];
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("LineByIndex[%d] is not valid"), idx);
	return FText();
}