// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAIInteractableActor.h"
#include "Components/BoxComponent.h"

// Sets default values
ARAIInteractableActor::ARAIInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	//추후 Collision Preset 설정해주기
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

void ARAIInteractableActor::Interacted()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Interacted"), *this->GetName());
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
}

void ARAIInteractableActor::DisableHighlight()
{
}