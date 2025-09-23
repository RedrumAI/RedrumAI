// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"  
#include "RAIEmotionScore.generated.h"

USTRUCT(BlueprintType)
struct FRAIEmotionScore : public FTableRowBase
{
	GENERATED_BODY()

public:

	FRAIEmotionScore();
	~FRAIEmotionScore();

	//¹üÀ§ = clamp(9.99, 0.00)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Love;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Joy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Surprise;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Anger;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Fear;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Sadness;
};
