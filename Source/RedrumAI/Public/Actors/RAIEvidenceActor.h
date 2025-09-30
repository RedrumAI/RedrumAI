// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/RAIInteractableActor.h"
#include "Engine/DataTable.h"
#include "RAIEvidenceActor.generated.h"

UCLASS()
class REDRUMAI_API ARAIEvidenceActor : public ARAIInteractableActor
{
	GENERATED_BODY()
	
public:
	ARAIEvidenceActor();

	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	UPROPERTY(EditAnywhere, Category = "RAI", meta = (RowType = "RAIEvidenceData"))
	FDataTableRowHandle EvidenceDataRow;
	
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ARAIEvidenceActor> EvidenceClass;
	UPROPERTY(VisibleAnywhere)
	FText DisplayName;
	UPROPERTY(VisibleAnywhere)
	FText Description;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTexture2D> EvidenceImage;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	/*
	MeshComponent로 사용
	UPROPERTY(VisibleAnywhere)
	TSoftObjectPtr<UStaticMesh> Mesh;
	*/

public:
	TSubclassOf<ARAIEvidenceActor> GetEvidenceClass() const { return EvidenceClass; };
	FText GetDisplayName() const { return DisplayName; };
	FText GetDescription() const { return Description; };
	TSoftObjectPtr<UStaticMesh> GetMesh() const { return MeshComponent->GetStaticMesh(); };
	TObjectPtr<UTexture2D> GetEvidenceImage() const { return EvidenceImage; };		

	virtual void Interacted() override;
};
