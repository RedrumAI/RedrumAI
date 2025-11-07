// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAIInteractableActor.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInterface.h"

// Sets default values
ARAIInteractableActor::ARAIInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractType = ERAIInteractType::Unknown;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	OriginalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OriginalMesh"));
	OriginalMesh->SetupAttachment(CollisionBox);
	OutlineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OutlineMesh"));
	OutlineMesh->SetupAttachment(OriginalMesh);

	//추후 Collision Preset 설정해주기, 일단 TestBP에서 설정
	CollisionBox->SetGenerateOverlapEvents(true);
	OriginalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OutlineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OutlineMesh->SetCastShadow(false);
	OutlineMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void ARAIInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARAIInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARAIInteractableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const FSoftObjectPath OutlineMaterialPath(TEXT("/Script/Engine.Material'/Game/YJ/Actors/EvidenceActor/M_HighlightOutline.M_HighlightOutline'"));
	OutlineMaterial = Cast<UMaterialInterface>(OutlineMaterialPath.TryLoad());

	OutlineMesh->SetStaticMesh(OriginalMesh->GetStaticMesh());
	if (IsValid(OutlineMaterial))
	{
		OutlineMesh->SetMaterial(0, OutlineMaterial);
	}
}

void ARAIInteractableActor::Interacted()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Interacted"), *this->GetName());
}

void ARAIInteractableActor::Interacted(AController* InController)
{
}

void ARAIInteractableActor::BeginFocused()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Begin Focused"), *this->GetName());

}

void ARAIInteractableActor::EndFocused()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] End Focused"), *this->GetName());
}

void ARAIInteractableActor::EnableHighlight()
{

	UE_LOG(LogTemp, Warning, TEXT("EnableHighlight On"));
	OutlineMesh->SetVisibility(true);
}

void ARAIInteractableActor::DisableHighlight()
{
	UE_LOG(LogTemp, Warning, TEXT("DisableHighlight"));
	OutlineMesh->SetVisibility(false);
}

ERAIInteractType ARAIInteractableActor::GetInteractType()
{
	return InteractType;
}
