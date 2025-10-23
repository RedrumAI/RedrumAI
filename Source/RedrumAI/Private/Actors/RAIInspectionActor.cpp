// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAIInspectionActor.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"

ARAIInspectionActor::ARAIInspectionActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//각 컴포넌트 위치는 BP에서 조정
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(RootComponent);
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(RootComponent);
	//SceneCaptureComponent2D->TextureTarget은 BP에서 할당
	
	MeshComponent->SetLightingChannels(false, true, false);
	PointLight->SetLightingChannels(false, true, false);
}

void ARAIInspectionActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld()) //블루프린트 뷰포트 확인용 if문
	{
		MeshComponent->SetVisibleInSceneCaptureOnly(true);
		SceneCaptureComponent2D->ShowOnlyComponent(MeshComponent);
	}
}

void ARAIInspectionActor::BeginPlay()
{
	Super::BeginPlay();

}

void ARAIInspectionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

