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

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARAIEvidenceActor> EvidenceClass;
	UPROPERTY(EditAnywhere)
	ERAIEvidenceType EvidenceType;
	UPROPERTY(EditAnywhere)
	FText DisplayName;
	UPROPERTY(EditAnywhere)
	FText Description;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> EvidenceImage;
};
