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
	FName Name;
	TObjectPtr<UTexture2D> SuspectImage;
	uint8 bIsAnswer : 1 = 0;

public:
	FName GetName() { return Name; }
	void SetName(FName InName) { Name = InName; }
	UTexture2D* GetSuspectImage() { return SuspectImage; }
	void SetSuspectImage(UTexture2D* InSuspectImage) { SuspectImage = InSuspectImage; }
	bool GetIsAnswer() { return bIsAnswer; }
	void SetIsAnswer(bool InIsAnswer) { bIsAnswer = InIsAnswer; }
};
