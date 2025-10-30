// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAIInspectionActor.h"
#include "Components/ArrowComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"

ARAIInspectionActor::ARAIInspectionActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//각 컴포넌트 위치는 BP에서 조정
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(MeshComponent);
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(RootComponent);
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(RootComponent);
	//SceneCaptureComponent2D->TextureTarget은 BP에서 할당

	MeshComponent->SetLightingChannels(false, true, false);
	PointLight->SetLightingChannels(false, true, false);

	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshComponent->SetVisibleInSceneCaptureOnly(true);
	SceneCaptureComponent2D->ShowOnlyComponent(MeshComponent);

	FRotator InitialActorRotator = MeshComponent->GetRelativeRotation();
	FVector InitialCameraVector = SceneCaptureComponent2D->GetRelativeLocation();
	InitialYaw = InitialActorRotator.Yaw;
	InitialPitch = InitialActorRotator.Pitch;
	InitialDistance = InitialCameraVector.X;

	YawSpeed = 0.2f;
	PitchSpeed = 0.2f;
	WheelSpeed = 10.f;
	MinDistance = 40.f;
	MaxDistance = 200.f;
}

void ARAIInspectionActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	/*
	MeshComponent->SetRelativeRotation(FRotator(InitialPitch, InitialYaw, 0));
	SceneCaptureComponent2D->SetRelativeRotation(FRotator(0, 180.f, 0));
	SceneCaptureComponent2D->SetRelativeLocation(FVector(InitialDistance, 0, 0));
	*/
}

void ARAIInspectionActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("yaw = %f  pitch = %f distance = %f"), InitialYaw, InitialPitch, InitialDistance);
}

void ARAIInspectionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARAIInspectionActor::AddYawPitch(float DeltaYaw, float DeltaPitch)
{
	UE_LOG(LogTemp, Warning, TEXT("InspectionActor AddYawPitch Yaw: %f, Pitch: %f"), DeltaYaw, DeltaPitch)
	MeshComponent->AddRelativeRotation(FRotator(DeltaPitch * PitchSpeed, DeltaYaw * YawSpeed, 0));
}

void ARAIInspectionActor::UpdateActorMesh(UStaticMesh* InMesh)
{
	if (IsValid(InMesh))
	{
		MeshComponent->SetStaticMesh(InMesh);
	}	
}

void ARAIInspectionActor::AddZoom(float DeltaWheel)
{
	float Distance = SceneCaptureComponent2D->GetRelativeLocation().X;
	Distance -= DeltaWheel * WheelSpeed;
	Distance = FMath::Clamp(Distance, MinDistance, MaxDistance);

	SceneCaptureComponent2D->SetRelativeLocation(FVector(Distance,0,0));
}

void ARAIInspectionActor::ResetTransform()
{
	MeshComponent->SetRelativeRotation(FRotator(InitialPitch, InitialYaw, 0));
	SceneCaptureComponent2D->SetRelativeLocation(FVector(InitialDistance), 0, 0);
}

