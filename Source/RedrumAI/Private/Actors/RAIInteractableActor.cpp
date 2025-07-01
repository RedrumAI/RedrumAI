// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAIInteractableActor.h"
#include "Components/SphereComponent.h"
#include ""

// Sets default values
ARAIInteractableActor::ARAIInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	//SphereCollision Radius설정은 BP에서 직접 설정하자.

}

// Called when the game starts or when spawned
void ARAIInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ARAIInteractableActor::OnBeginOverlapped);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ARAIInteractableActor::OnEndOverlapped);
}

// Called every frame
void ARAIInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARAIInteractableActor::Interacted()
{
}

void ARAIInteractableActor::OnBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if OhterActor 클래스가 RAIPlayerCharacter인지 확인(UClass이용하면 cast하지 않아도 될지도)

	EnableHighlight();
	CastedOtherActor->EnableLinetrace();
}

void ARAIInteractableActor::OnEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if OhterActor 클래스가 RAIPlayerCharacter인지 확인(UClass이용하면 cast하지 않아도 될지도)
	DisableHighlight();
}

void ARAIInteractableActor::EnableHighlight()
{
}

void ARAIInteractableActor::DisableHighlight()
{
}

void ARAIInteractableActor::EnablePlayerLinetrace()
{
	//접촉한 대상이 player인지 ? Linetrace On : NULL
	
}

void ARAIInteractableActor::DIsablePlayerLinetrace()
{
	//접촉한 대상이 player인지 ? Linetrace Off : NULL
}

