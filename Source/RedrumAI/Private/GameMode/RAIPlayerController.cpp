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

	for (TFieldIterator<FObjectProperty> PropertyIterator(GetClass()); PropertyIterator; ++PropertyIterator)
	{
		FObjectProperty* ObjectProperty = *PropertyIterator;
		if (ObjectProperty->PropertyClass == UInputMappingContext::StaticClass())
		{
			UInputMappingContext* IMCProperty = Cast<UInputMappingContext>(ObjectProperty->GetObjectPropertyValue_InContainer(this));

			if (IMCProperty)
			{
				IMCArray.AddUnique(IMCProperty);
			}
		}
	}

	//BP로 만들어진 StageHUD의 경로 하드코딩
	FSoftClassPath StageHUDClassPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/YJ/Widget/WBP_StageHUDWidget.WBP_StageHUDWidget_C'"));
	UClass* WidgetClass = StageHUDClassPath.TryLoadClass<URAIStageHUDWidget>();
	StageHUD = CreateWidget<URAIStageHUDWidget>(this, WidgetClass);
	StageHUD->AddToViewport();

	//EnhancedInputLocalPlayerSubsystem과 InputMapping 연결
	EnterDefaultModeIMC();

	BindGM();
	BindHUD();
	UpdateTalkingStateDelegate.AddDynamic(this, &ARAIPlayerController::SwitchTalkingMode);

	//InspectionActor생성 및 리셋함수 바인드
	InspectionActor= GetWorld()->SpawnActor<ARAIInspectionActor>(BP_InspectionActor);
	if (InspectionActor)
	{
		ResetInspectionMeshDelegate.AddDynamic(InspectionActor, &ARAIInspectionActor::ResetMeshTransform);
	}
}

void ARAIPlayerController::BindGM()
{
	RAIGameMode = Cast<ARAIGameMode>(UGameplayStatics::GetGameMode(this));
	ensure(RAIGameMode);

	RAIGameMode->SendResponseDelegate.AddDynamic(this, &ARAIPlayerController::SetAIChat);
	RAIGameMode->UpdateChatLogUIDelegate.AddDynamic(this, &ARAIPlayerController::AddChatLogUI);
}

void ARAIPlayerController::BindHUD()
{
	if (IsValid(StageHUD))
	{
		UpdateTalkingStateDelegate.AddDynamic(StageHUD, &URAIStageHUDWidget::SwitchChatUI);
	}
	else
	{
		FTimerHandle TimerHandle_BindHUD;
		//TODO: 타이머를 통해 0.1초 뒤에 BindHM() 다시 실행.
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_BindHUD,
			this,
			&ARAIPlayerController::BindHUD,
			0.1f,
			false
		);
	}
}

void ARAIPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	// 여기에서 'ETriggerEvent' 열거형 값을 변경하여 원하는 트리거 이벤트를 바인딩할 수 있습니다.
	Input->BindAction(IA_ToggleMouseCursor, ETriggerEvent::Triggered, this, &ARAIPlayerController::ToggleMouseCursor);
	Input->BindAction(IA_CloseLastUI, ETriggerEvent::Triggered, this, &ARAIPlayerController::CloseLastUI);
	Input->BindAction(IA_MoveSlideInventory, ETriggerEvent::Triggered, this, &ARAIPlayerController::MoveSlideInventory);

	Input->BindAction(IA_CloseInspectionUI, ETriggerEvent::Triggered, this, &ARAIPlayerController::CloseInspectionUI);
	Input->BindAction(IA_ResetInspectionUI, ETriggerEvent::Triggered, this, &ARAIPlayerController::ResetInspectionMesh);
}


void ARAIPlayerController::EnterDefaultModeIMC()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IsValid(IMC_DefaultMode))
			{
				//InputSystem->ClearAllMappings();
				for (UInputMappingContext* IMC : IMCArray)
				{
					if (IsValid(IMC))
					{
						InputSystem->RemoveMappingContext(IMC);
					}					
				}

				InputSystem->AddMappingContext(IMC_DefaultMode, 0);
			}
		}
	}
}

void ARAIPlayerController::ToggleMouseCursor()
{
	if (bShowMouseCursor)
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		SetShowMouseCursor(false);

		if (!bIsTalking)
		{
			GetPawn()->EnableInput(this);
		}		
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

void ARAIPlayerController::EnterInspectionModeIMC()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IsValid(IMC_InspectionMode))
			{
				for (UInputMappingContext* IMC : IMCArray)
				{
					if (IsValid(IMC))
					{
						InputSystem->RemoveMappingContext(IMC);
					}
				}
				InputSystem->AddMappingContext(IMC_InspectionMode, 0);
			}
		}
	}
}

void ARAIPlayerController::CloseInspectionUI()
{
	CloseInspectionUIDelegate.Broadcast();
}

void ARAIPlayerController::ResetInspectionMesh()
{
	ResetInspectionMeshDelegate.Broadcast();
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

bool ARAIPlayerController::GetTalkingState() const
{
	return bIsTalking;
}

void ARAIPlayerController::SetTalkingState(bool InBool)
{
	//변경될 경우에만 broadcast
	if (bIsTalking != InBool)
	{
		bIsTalking = InBool;
		UpdateTalkingStateDelegate.Broadcast(bIsTalking);
	}	
}

void ARAIPlayerController::SwitchTalkingMode(bool InBool)
{
	if (InBool)
	{
		GetPawn()->DisableInput(this);
	}
	else
	{
		GetPawn()->EnableInput(this);
	}
}

void ARAIPlayerController::AskSuspect(FText Text)
{
	RAIGameMode->AskSuspect(Text);
}

void ARAIPlayerController::UseEvidence(FName InRowName)
{
	RAIGameMode->UpdateEvidence(InRowName, EUpdateType::Remove);
}

const ARAIInspectionActor* ARAIPlayerController::GetInspectionActor()
{
	return InspectionActor;
}
