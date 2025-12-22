// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"  
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
	//레벨데이터 //지금은 단일 레벨이라 pass

	//레벨스토리 UI입력용
	//UPROPERTY()
	//FText LevelStory; //단일 레벨인 지금은 pass

	//레벨 시작 시퀀스
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<ULevelSequence> IntroSequenceAsset;

	// 레벨에 등장하는 용의자들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RAI", meta = (RowType = "/Script/RedrumAI.RAIFinalVerdictDataStruct")) //메타데이터를 통해 DT와 내부 핸들을 드롭다운으로 노출
	TArray<FDataTableRowHandle> Suspects;

	// 범인
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RAI", meta = (RowType = "/Script/RedrumAI.RAIFinalVerdictDataStruct"))
	FDataTableRowHandle Answer;

};
