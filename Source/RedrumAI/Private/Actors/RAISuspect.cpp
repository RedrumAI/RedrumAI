// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAISuspect.h"
#include "GameMode/RAIPlayerController.h"

ARAISuspect::ARAISuspect()
{
	EvidenceType = ERAIEvidenceType::NotEvidence;
}

void ARAISuspect::Interacted(AController* InController)
{
	UE_LOG(LogTemp, Warning, TEXT("!"));

	ARAIPlayerController* InPC = Cast<ARAIPlayerController>(InController);
	InPC->SetTalkingState(true);
}
