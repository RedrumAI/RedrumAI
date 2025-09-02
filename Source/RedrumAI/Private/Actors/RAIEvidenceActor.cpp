// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAIEvidenceActor.h"
#include "Data/RAIEvidenceData.h"
#include "Components/StaticMeshComponent.h"
#include "GameMode/RAIGameMode.h"

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

void ARAIEvidenceActor::Interacted()
{
	//TODO: 플레이어의 IM에게 신호보내는 함수 호출, 인자로 EvidenceDataRow.RowName 보내기
	ARAIGameMode* RAIGameMode= Cast<ARAIGameMode>(GetWorld()->GetAuthGameMode());
	RAIGameMode->AddEvidence(EvidenceDataRow.RowName);
}
