// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAIEvidenceActor.h"
#include "Data/RAIEvidenceData.h"
#include "Components/StaticMeshComponent.h"

ARAIEvidenceActor::ARAIEvidenceActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

void ARAIEvidenceActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FString DebugContext = FString::Printf(TEXT("[%s] : EvidenceData GetRow Called"), *GetName());
	const FRAIEvidenceData* EvidenceData = EvidenceDataRow.GetRow<FRAIEvidenceData>(DebugContext);
	if (EvidenceData != nullptr)
	{
		EvidenceClass = EvidenceData->EvidenceClass;
		EvidenceType = EvidenceData->EvidenceType;
		DisplayName = EvidenceData->DisplayName;
		Description = EvidenceData->Description;
		EvidenceImage = EvidenceData->EvidenceImage;
		MeshComponent->SetStaticMesh(EvidenceData->Mesh);
	}
}
