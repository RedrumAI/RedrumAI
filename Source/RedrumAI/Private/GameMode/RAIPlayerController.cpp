// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIPlayerController.h"
#include "UI/RAIStageHUDWidget.h"
#include "GameMode/RAIGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Actors/RAIInspectionActor.h"
#include "Actors/RAIConversationInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "UI/RAILobbyUI.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

ARAIPlayerController::ARAIPlayerController()
{
}

void ARAIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 소유IMC 확인
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

	//InspectionActor생성 및 리셋함수 바인드
	InspectionActor = GetWorld()->SpawnActor<ARAIInspectionActor>(BP_InspectionActor);
	if (InspectionActor)
	{
		ResetInspectionMeshDelegate.AddDynamic(InspectionActor, &ARAIInspectionActor::ResetMeshTransform);
	}

	//GM바인드
	BindGM();
	
	//TalkingState 관리 함수 bind
	UpdateTalkingStateDelegate.AddDynamic(this, &ARAIPlayerController::SwitchTalkingMode);

	//Intro관련 설정
	FindStageTargetPoint();

	if (LobbyUIClass)
	{
		LobbyUI = CreateWidget<URAILobbyUI>(this, LobbyUIClass);
		if (LobbyUI)
		{
			LobbyUI->AddToViewport();
		}
	}

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
	if (APawn* MyPawn = GetPawn())
	{
		MyPawn->DisableInput(this);
	}
}

void ARAIPlayerController::FindStageTargetPoint()
{
	for (TActorIterator<ATargetPoint> TargetPointIterator(GetWorld()); TargetPointIterator; ++TargetPointIterator)
	{
		ATargetPoint* TargetPoint = *TargetPointIterator;
		if (TargetPoint && TargetPoint->ActorHasTag(FName("StageTargetPoint")))
		{
			StageTargetPoint = TargetPoint;
			break;
		}
	}
}

void ARAIPlayerController::GameStartFromLobby()
{
	//LobbyUI 제거
	if (LobbyUI)
	{
		LobbyUI->RemoveFromParent();
		LobbyUI = nullptr;
	}

	//시퀀스 재생하는 동안 입력 차단
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(false);

	PlayIntroSequence();
}

void ARAIPlayerController::PlayIntroSequence()
{
	if (!IntroSequenceAsset)
	{
		SetupStageAfterIntro();
		return;
	}

	FMovieSceneSequencePlaybackSettings Settings;
	ALevelSequenceActor* SequenceActor = nullptr;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		IntroSequenceAsset,
		Settings,
		SequenceActor
	);

	if (SequencePlayer)
	{
		SequencePlayer->OnFinished.AddDynamic(this, &ARAIPlayerController::SetupStageAfterIntro);
		SequencePlayer->Play();
	}
}

void ARAIPlayerController::SetupStageAfterIntro()
{
	GetPawn()->SetActorLocation(StageTargetPoint->GetActorLocation());
	GetPawn()->EnableInput(this);

	if (StageHUDClass)
	{
		StageHUD = CreateWidget<URAIStageHUDWidget>(this, StageHUDClass);
		if (StageHUD)
		{
			StageHUD->AddToViewport();
		}
	}

	//EnhancedInputLocalPlayerSubsystem과 InputMapping 연결
	EnterDefaultModeIMC();

	BindHUD();
}

void ARAIPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (ApproachState != ERAIConversationApproachState::None)
	{
		TickConversationApproach(DeltaTime);
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
				InputSystem->RequestRebuildControlMappings();
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
				InputSystem->RequestRebuildControlMappings();
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

void ARAIPlayerController::TryStartConversation(IRAIConversationInterface* Suspect)
{
	if (!Suspect) return;
	if (bIsTalking || ApproachState != ERAIConversationApproachState::None) return;

	APawn* PlayerPawn = GetPawn();
	if (!PlayerPawn) return;

	CurrentSuspect = Suspect;

	SetTalkingState(true);

	const ERAIConversationSide Side = Suspect->GetConversationSide(this);

	switch (Side)
	{
	case ERAIConversationSide::Front:
		StartApproachToFront(Suspect);
		break;

	case ERAIConversationSide::Left:
		StartApproachToSide(Suspect, false);
		break;

	case ERAIConversationSide::Right:
		StartApproachToSide(Suspect, true);
		break;
	}
}

void ARAIPlayerController::StartApproachToFront(IRAIConversationInterface* Suspect)
{
	APawn* PlayerPawn = GetPawn();
	if (!PlayerPawn || !Suspect) return;

	ApproachElapsedTime = 0.f;
	ApproachDuration = 0.4f;

	ApproachStartLocation = PlayerPawn->GetActorLocation();
	ApproachTargetLocation = Suspect->GetFrontAnchorLocation();

	ApproachState = ERAIConversationApproachState::ToFrontAnchor;
}

void ARAIPlayerController::StartApproachToSide(IRAIConversationInterface* Suspect, bool bUseRight)
{
	APawn* PlayerPawn = GetPawn();
	if (!PlayerPawn || !Suspect) return;

	ApproachElapsedTime = 0.f;
	ApproachDuration = 0.4f;

	ApproachStartLocation = PlayerPawn->GetActorLocation();
	ApproachTargetLocation = bUseRight ? Suspect->GetRightAnchorLocation() : Suspect->GetLeftAnchorLocation();

	//TickConversationApproach에게 끝나고 ToFrontAnchor로 가야한다 알리기 위해 현재상태 표시
	ApproachState = ERAIConversationApproachState::ToSideAnchor;
}

void ARAIPlayerController::TickConversationApproach(float DeltaSeconds)
{
	APawn* PlayerPawn = GetPawn();
	if (!PlayerPawn || !CurrentSuspect)
	{
		ApproachState = ERAIConversationApproachState::None;
		return;
	}

	ApproachElapsedTime += DeltaSeconds;
	const float Alpha = FMath::Clamp(ApproachElapsedTime / ApproachDuration, 0.f, 1.f);

	// 위치 보간 : X/Y만 앵커를 향해 이동, Z는 현재 값 유지
	FVector NewLocation;
	NewLocation.X = FMath::Lerp(ApproachStartLocation.X, ApproachTargetLocation.X, Alpha);
	NewLocation.Y = FMath::Lerp(ApproachStartLocation.Y, ApproachTargetLocation.Y, Alpha);
	const FVector CurrentPawnLocation = PlayerPawn->GetActorLocation();
	NewLocation.Z = CurrentPawnLocation.Z;	// Z는 보간하지 않고 고정 (캐릭터 높이 유지)
	PlayerPawn->SetActorLocation(NewLocation);

	// 카메라를 용의자 머리 쪽으로 회전
	const FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	const FVector SuspectHeadLocation = CurrentSuspect->GetHeadWorldLocation();

	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, SuspectHeadLocation);
	const FRotator NewRotation = FMath::RInterpTo(GetControlRotation(), TargetRotation, DeltaSeconds, 50.f);
	SetControlRotation(NewRotation);

	// 보간 끝났으면 상태 전환
	if (Alpha >= 1.f)
	{
		if (ApproachState == ERAIConversationApproachState::ToSideAnchor)
		{
			// 옆 앵커 도착 -> 정면 앵커로 한 번 더 이동
			StartApproachToFront(CurrentSuspect);
		}
		else // ToFrontAnchor 완료
		{
			ApproachState = ERAIConversationApproachState::None;

			// 마지막으로 한 번 더 정확히 정면을 바라보도록 정리
			const FVector FinalCameraLocation = PlayerCameraManager->GetCameraLocation();
			const FRotator FinalLook = UKismetMathLibrary::FindLookAtRotation(FinalCameraLocation, SuspectHeadLocation);
			SetControlRotation(FinalLook);
		}
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

