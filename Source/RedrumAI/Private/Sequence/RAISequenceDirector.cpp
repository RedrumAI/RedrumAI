// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/RAISequenceDirector.h"
#include "GameMode/RAIGameState.h"
#include "GameMode/RAIPlayerController.h"
#include "UI/RAIEndingHUD.h"
#include "LevelSequencePlayer.h"

void URAISequenceDirector::Initialize()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	GameState = World->GetGameState<ARAIGameState>();
}

void URAISequenceDirector::TriggerDialogue(int idx)
{
	GameState->TriggerDialogue(idx);
}

void URAISequenceDirector::CheckEndingHold()
{
	if(!GetWorld())
	{
		return;
	}
	
	ARAIPlayerController* MyPC = GetWorld()->GetFirstPlayerController<ARAIPlayerController>();
	URAIEndingHUD* EndingHUD = MyPC->GetEndingHUD();
	if (!IsValid(EndingHUD))
	{
		return;
	}

	if(EndingHUD->bCreditAnimationEnd == true)
	{
		//DoNothing. 그대로 종결까지 플레이
	}
	else
	{
		//스탭롤의 이벤트 끝과 Hold풀어주기 함수를 바인드
		Player->Pause();
		EndingHUD->FinishEndingCreditAnimationDelegate.AddDynamic(this, &URAISequenceDirector::ReleaseEndingHold);
	}
}

void URAISequenceDirector::ReleaseEndingHold()
{
	Player->Play();
}