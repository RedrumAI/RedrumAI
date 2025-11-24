// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAILobbyUI.h"
#include "GameMode/RAIPlayerController.h"
#include "Components/Button.h"

void URAILobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	ARAIPlayerController* RAIPlayerController = Cast<ARAIPlayerController>(GetOwningPlayer());

	Button_Start = Cast<UButton>(GetWidgetFromName(TEXT("Button_Start")));
	Button_Exit = Cast<UButton>(GetWidgetFromName(TEXT("Button_Exit")));

	if (RAIPlayerController)
	{
		Button_Start->OnClicked.AddDynamic(RAIPlayerController, &ARAIPlayerController::GameStartFromLobby);
	}
}
