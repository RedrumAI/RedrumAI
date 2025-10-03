// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAISuspect.h"
#include "GameMode/RAIPlayerController.h"

ARAISuspect::ARAISuspect()
{
	InteractType = ERAIInteractType::Suspect;
}

void ARAISuspect::Interacted(AController* InController)
{
	ARAIPlayerController* InPC = Cast<ARAIPlayerController>(InController);
	InPC->SetTalkingState(true);
}
