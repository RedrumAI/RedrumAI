// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"  
#include "RAIFinalVerdictDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FRAIFinalVerdictDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	FRAIFinalVerdictDataStruct();
	~FRAIFinalVerdictDataStruct();

	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> SuspectImage;
};
