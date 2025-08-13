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

	UPROPERTY()
	TSubclassOf<ARAIEvidenceActor> EvidenceClass;
	UPROPERTY()
	ERAIEvidenceType EvidenceType;
	UPROPERTY()
	FText DisplayName;
	UPROPERTY()
	FText Description;
	UPROPERTY()
	TObjectPtr<UStaticMesh> Mesh;
	UPROPERTY()
	TObjectPtr<UTexture2D> EvidenceImage;
};
