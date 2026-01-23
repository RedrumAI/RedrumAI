// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "Data/RAILevelSuspectData.h"
#include "RAILevelDataStruct.generated.h"

class ULevelSequence;

USTRUCT(BlueprintType)
struct FRAILevelDataStruct : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FRAILevelDataStruct();
	~FRAILevelDataStruct();

	//UPROPERTY()
	//ULevel* LevelToTravel //지금은 단일 레벨이라 pass

	//레벨스토리 UI입력용
	//UPROPERTY()
	//FText LevelStory; //단일 레벨인 지금은 pass

	//레벨 시작 시퀀스
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<ULevelSequence> IntroSequenceAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RAI")
	TArray<FRAILevelSuspectData> LevelSuspects;
	// 범인
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RAI", meta = (RowType = "/Script/RedrumAI.RAIFinalVerdictDataStruct"))
	FDataTableRowHandle Answer;
	
};
