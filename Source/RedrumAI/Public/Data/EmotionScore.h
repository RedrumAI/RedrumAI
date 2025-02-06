// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmotionScore.generated.h"

USTRUCT(BlueprintType)
struct FEmotionScore
{
	GENERATED_BODY()

public:
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
