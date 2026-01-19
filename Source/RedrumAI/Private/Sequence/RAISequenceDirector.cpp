// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/RAISequenceDirector.h"
#include "GameMode/RAIGameState.h"

void URAISequenceDirector::Initialize()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	GameState = World->GetGameState<ARAIGameState>();
}

void URAISequenceDirector::SayHi()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi"));
}