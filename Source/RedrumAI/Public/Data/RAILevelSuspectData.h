#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"  
#include "RAILevelSuspectData.generated.h"

class ULevelSequence;
class URAICutsceneDialogueDataAsset;

USTRUCT(BlueprintType)
struct FRAILevelSuspectData : public FTableRowBase
{
	GENERATED_BODY()

	public:
	FRAILevelSuspectData();
	~FRAILevelSuspectData();
	
	//메타데이터를 통해 DT와 내부 핸들을 드롭다운으로 노출
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RAI", meta = (RowType = "/Script/RedrumAI.RAIFinalVerdictDataStruct"))
	FDataTableRowHandle Suspect;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<ULevelSequence> EndingSequence;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<URAICutsceneDialogueDataAsset> CutsceneDialogue;
};
