// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/RAIFinalVerdictDataStruct.h"
#include "RAIFinalVerdictDataObject.generated.h"

/**
 * 
 */
UCLASS()
class REDRUMAI_API URAIFinalVerdictDataObject : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	FRAIFinalVerdictDataStruct Data;
	UPROPERTY(EditAnywhere)
	uint8 bIsAnswer = 0;
	
public:
	FRAIFinalVerdictDataStruct GetData() { return Data; }
	void SetData(FRAIFinalVerdictDataStruct InData) { Data = InData; }
	FName GetName() { return Data.Name; }
	void SetName(FName InName) { Data.Name = InName; }
	UTexture2D* GetSuspectImage() { return Data.SuspectImage; }
	void SetSuspectImage(UTexture2D* InSuspectImage) { Data.SuspectImage = InSuspectImage; }
};
