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
	
	MeshComponent->SetOnlyOwnerSee(true);
	MeshComponent->SetLightingChannels(false, true, false);
	PointLight->SetLightingChannels(false, true, false);
	SceneCaptureComponent2D->ShowOnlyComponent(MeshComponent);
	//SceneCaptureComponent2D->TextureTarget은 BP에서 할당
}

void ARAIInspectionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARAIInspectionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

