// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIPlayerController.h"
#include "UI/RAIStageHUD.h"
#include "UI/RAIChatUI.h"
#include "GameMode/RAIGameMode.h"
#include "Kismet/GameplayStatics.h"

ARAIPlayerController::ARAIPlayerController()
{
}

void ARAIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//BP로 만들어진 StageHUD의 경로 하드코딩
	FSoftClassPath StageHUDClassPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/YJ/Widget/WBP_StageHUD.WBP_StageHUD_C'"));
	UClass* WidgetClass = StageHUDClassPath.TryLoadClass<URAIStageHUD>();
	StageHUD = CreateWidget<URAIStageHUD>(this, WidgetClass);
	StageHUD->AddToViewport();
	//TODO:: StageHUD->ChatUI가 존재하는지 여부확인필요한데, 여기는 포인터라 그냥둬도 될지도?
	ChatUI = StageHUD->ChatUI;

	RAIGameMode = Cast<ARAIGameMode>(UGameplayStatics::GetGameMode(this));
	ensure(RAIGameMode);
	RAIGameMode->SendResponseDelegate.AddDynamic(this, &ARAIPlayerController::AddAIChat);
}

void ARAIPlayerController::AddAIChat(FString String)
{
	if (IsValid(ChatUI))
	{
		ChatUI->SetAIChat(String);
	}	
}

void ARAIPlayerController::AskSuspect(FText Text)
{
	RAIGameMode->AskSuspect(Text);
}
