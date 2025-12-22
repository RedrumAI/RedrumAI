// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAILobbyUI.h"
#include "GameMode/RAIPlayerController.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"

void URAILobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	ARAIPlayerController* RAIPlayerController = Cast<ARAIPlayerController>(GetOwningPlayer());

	Button_Start = Cast<UButton>(GetWidgetFromName(TEXT("Button_Start")));
	Button_Exit = Cast<UButton>(GetWidgetFromName(TEXT("Button_Exit")));

	VerticalBox_Level = Cast<UVerticalBox>(GetWidgetFromName(TEXT("VerticalBox_Level")));
	Button_Level1 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Level1")));
	Button_Level2 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Level2")));
	Button_Level3 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Level3")));

	Button_Start->OnClicked.AddDynamic(this, &URAILobbyUI::ToggleLevelVerticalBox);

	Button_Level1->OnClicked.AddDynamic(this, &URAILobbyUI::OnClick_Level1);
	Button_Level1->OnClicked.AddDynamic(this, &URAILobbyUI::OnClick_Level2);
	Button_Level1->OnClicked.AddDynamic(this, &URAILobbyUI::OnClick_Level3);
}

void URAILobbyUI::ToggleLevelVerticalBox()
{
	VerticalBox_Level->SetVisibility(
		VerticalBox_Level->GetVisibility() == ESlateVisibility::Visible 
		? ESlateVisibility::Collapsed : ESlateVisibility::Visible
	);
}

void URAILobbyUI::SetupLevelByRowName(FName InRowName)
{
	ARAIPlayerController* RAIPlayerController = Cast<ARAIPlayerController>(GetOwningPlayer());
	RAIPlayerController->SetupLevelByRowName(InRowName);
}
