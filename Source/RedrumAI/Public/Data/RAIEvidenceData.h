// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"  
#include "Data/RAIEvidenceType.h"
#include "RAIEvidenceData.generated.h"

class ARAIEvidenceActor;

USTRUCT(BlueprintType)
struct FRAIEvidenceData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FRAIEvidenceData();
	~FRAIEvidenceData();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ARAIEvidenceActor> EvidenceClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ERAIEvidenceType EvidenceType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DisplayName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> EvidenceImage;
	
};
