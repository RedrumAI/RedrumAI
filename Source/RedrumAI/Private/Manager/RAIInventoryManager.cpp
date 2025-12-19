// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RAIInventoryManager.h"
#include "GameMode/RAIGameState.h"

// Sets default values
ARAIInventoryManager::ARAIInventoryManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ARAIInventoryManager::AddEvidence(FName EvidenceRowName)
{
	ARAIGameState* RAIGameState = GetWorld()->GetGameState<ARAIGameState>();
	RAIGameState->AddEvidence(EvidenceRowName);
}

void ARAIInventoryManager::RemoveEvidence(FName EvidenceRowName)
{
	ARAIGameState* RAIGameState = GetWorld()->GetGameState<ARAIGameState>();
	RAIGameState->RemoveEvidence(EvidenceRowName);
}
