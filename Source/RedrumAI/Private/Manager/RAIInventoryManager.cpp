// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RAIInventoryManager.h"
#include "GameMode/RAIPlayerState.h"

// Sets default values
ARAIInventoryManager::ARAIInventoryManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARAIInventoryManager::BeginPlay()
{
	Super::BeginPlay();

	InitInventoryManager();

}

void ARAIInventoryManager::InitInventoryManager()
{
	// RAIPlayerState 조회 후 할당
	RAIPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<ARAIPlayerState>();
	if (!IsValid(RAIPlayerState))
	{
		FTimerHandle TimerHandle_InitInventoryManager;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_InitInventoryManager,
			this,
			&ARAIInventoryManager::InitInventoryManager,
			0.1f,
			false
		);
	}
}

// Called every frame
void ARAIInventoryManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARAIInventoryManager::AddEvidence(FName EvidenceRowName)
{
	//TODO: PS의 컨테이너에 Add함수 트리거
	RAIPlayerState->AddEvidence(EvidenceRowName);
}
