// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIPlayerController.h"
#include "UI/RAIStageHUDWidget.h"
#include "GameMode/RAIGameMode.h"
#include "Kismet/GameplayStatics.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

ARAIPlayerController::ARAIPlayerController()
{
}

void ARAIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//BP로 만들어진 StageHUD의 경로 하드코딩
	FSoftClassPath StageHUDClassPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/YJ/Widget/WBP_StageHUDWidget.WBP_StageHUDWidget_C'"));
	UClass* WidgetClass = StageHUDClassPath.TryLoadClass<URAIStageHUDWidget>();
	StageHUD = CreateWidget<URAIStageHUDWidget>(this, WidgetClass);
	StageHUD->AddToViewport();

	//EnhancedInputLocalPlayerSubsystem과 InputMapping 연결
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))//현재 Controller에 연결된 Player가 LocalPlayer인지 확인하고
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) //그 로컬플레이어의 Subsystem가져오기
		{
			if (IsValid(InputMapping))
			{
				InputSystem->AddMappingContext(InputMapping, 0);
			}
		}
	}

	BindGM();
}

void ARAIPlayerController::BindGM()
{
	RAIGameMode = Cast<ARAIGameMode>(UGameplayStatics::GetGameMode(this));
	ensure(RAIGameMode);

	RAIGameMode->SendResponseDelegate.AddDynamic(this, &ARAIPlayerController::SetAIChat);
	RAIGameMode->UpdateChatLogUIDelegate.AddDynamic(this, &ARAIPlayerController::AddChatLogUI);
}

void ARAIPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	// 여기에서 'ETriggerEvent' 열거형 값을 변경하여 원하는 트리거 이벤트를 바인딩할 수 있습니다.
	Input->BindAction(IA_ToggleMouseCursor, ETriggerEvent::Triggered, this, &ARAIPlayerController::ToggleMouseCursor);
	Input->BindAction(IA_CloseLastUI, ETriggerEvent::Triggered, this, &ARAIPlayerController::CloseLastUI);
	Input->BindAction(IA_MoveSlideInventory, ETriggerEvent::Triggered, this, &ARAIPlayerController::MoveSlideInventory);
}

void ARAIPlayerController::ToggleMouseCursor()
{
	if (bShowMouseCursor)
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		SetShowMouseCursor(false);

		GetPawn()->EnableInput(this);
	}
	else
	{
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);

		SetShowMouseCursor(true);

		GetPawn()->DisableInput(this);
	}
}

void ARAIPlayerController::CloseLastUI()
{
	if (IsValid(StageHUD))
	{
		StageHUD->CloseLastUI();
	}
}

void ARAIPlayerController::MoveSlideInventory()
{
	MoveSlideInventoryDelegate.Broadcast();
}

void ARAIPlayerController::SetAIChat(FString String)
{
	if (IsValid(StageHUD))
	{
		StageHUD->SetAIChat(String);
	}
}

void ARAIPlayerController::AddChatLogUI(FString InRole, FString InMessage)
{
	if (IsValid(StageHUD))
	{
		StageHUD->AddChatLogUI(InRole, InMessage);
	}
}

void ARAIPlayerController::AskSuspect(FText Text)
{
	RAIGameMode->AskSuspect(Text);
}
