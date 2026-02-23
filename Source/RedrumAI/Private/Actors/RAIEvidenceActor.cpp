// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAIEvidenceActor.h"
#include "Data/RAIEvidenceData.h"
#include "Components/StaticMeshComponent.h"
#include "GameMode/RAIGameMode.h"

ARAIEvidenceActor::ARAIEvidenceActor()
{
	//증거물 DT에서 컴포넌트값을 가져오기에, 생성자가 아닌 OnConstruction로 초기화 코드 이동
}

void ARAIEvidenceActor::OnConstruction(const FTransform& Transform)
{
	FString DebugContext = FString::Printf(TEXT("[%s] : EvidenceData GetRow Called"), *GetName());
	const FRAIEvidenceData* EvidenceData = EvidenceDataRow.GetRow<FRAIEvidenceData>(DebugContext);
	if (EvidenceData != nullptr)
	{
		InteractType = EvidenceData->InteractType;
		EvidenceClass = EvidenceData->EvidenceClass;
		DisplayName = EvidenceData->DisplayName;
		Description = EvidenceData->Description;
		EvidenceImage = EvidenceData->EvidenceImage;
		OriginalMesh->SetStaticMesh(EvidenceData->Mesh);
	}

	//OriginalMesh 설정이 우선시 되어야하기에 Super는 후순위로 미뤄짐
	Super::OnConstruction(Transform);
}

void ARAIEvidenceActor::BeginPlay()
{
	ARAIGameMode* RAIGameMode = Cast<ARAIGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(RAIGameMode))
	{
		RAIGameMode->RegisterEvidence(this);
		UE_LOG(LogTemp, Warning, TEXT("Actor Registered"));
	}

}

void ARAIEvidenceActor::SetActorActivate(bool InBool)
{
	SetActorHiddenInGame(!InBool); //인자 Not에 주의
	SetActorEnableCollision(InBool);
	SetActorTickEnabled(InBool);
}

void ARAIEvidenceActor::Interacted()
{
	ARAIGameMode* RAIGameMode= Cast<ARAIGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(RAIGameMode))
	{
		RAIGameMode->UpdateEvidence(EvidenceDataRow.RowName, EUpdateType::Add);

		SetActorActivate(false); //정상 사용 후 비활성화
	}
}
