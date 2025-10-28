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

	InitialYaw = 0.f;
	InitialPitch = -10.f;
	InitialDistance = 120.f;

	YawSpeed = 0.2f;
	PitchSpeed = 0.2f;
	WheelSpeed = 10.f;
	MinDistance = 40.f;
	MaxDistance = 200.f;
}

void ARAIInspectionActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld()) //블루프린트 뷰포트 확인용 if문
	{
		MeshComponent->SetVisibleInSceneCaptureOnly(true);
		SceneCaptureComponent2D->ShowOnlyComponent(MeshComponent);
	}

	MeshComponent->SetRelativeRotation(FRotator(InitialPitch, InitialYaw, 0));
	SceneCaptureComponent2D->SetRelativeRotation(FRotator(0, 180.f, 0));
	SceneCaptureComponent2D->SetRelativeLocation(FVector(InitialDistance, 0, 0));
	
}

void ARAIInspectionActor::BeginPlay()
{
	Super::BeginPlay();

}

void ARAIInspectionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARAIInspectionActor::AddYawPitch(float DeltaYaw, float DeltaPitch)
{
	MeshComponent->AddRelativeRotation(FRotator(DeltaPitch * PitchSpeed, DeltaYaw * YawSpeed, 0));
}

void ARAIInspectionActor::AddZoom(float DeltaWheel)
{
	float Distance = SceneCaptureComponent2D->GetRelativeLocation().X;
	Distance -= DeltaWheel * WheelSpeed;
	Distance = FMath::Clamp(Distance, MinDistance, MaxDistance);

	SceneCaptureComponent2D->SetRelativeLocation(FVector(Distance), 0, 0);
}

void ARAIInspectionActor::ResetTransform()
{
	MeshComponent->SetRelativeRotation(FRotator(InitialPitch, InitialYaw, 0));
	SceneCaptureComponent2D->SetRelativeLocation(FVector(InitialDistance), 0, 0);
}

