// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIInspectionUI.h"
#include "GameMode/RAIPlayerController.h"
#include "Actors/RAIInspectionActor.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputCoreTypes.h"


void URAIInspectionUI::NativeConstruct()
{
	DisplayNameTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_DisplayName")));
	DescriptionTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Description")));
	ResetManualTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_ResetManual")));
	ExitManualTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_ExitManual")));

	DisplayNameTextBlock->SetAutoWrapText(true);
	DescriptionTextBlock->SetAutoWrapText(true);
	ResetManualTextBlock->SetAutoWrapText(true);
	ExitManualTextBlock->SetAutoWrapText(true);

	RAIPlayerController = Cast<ARAIPlayerController>(GetOwningPlayer());
	RAIPlayerController->CloseInspectionUIDelegate.AddDynamic(this, &URAIInspectionUI::CloseInspectionUI);
	BindInspectionActor();
}


void URAIInspectionUI::BindInspectionActor()
{
	const ARAIInspectionActor* InspectionActor = RAIPlayerController->GetInspectionActor();
	if (IsValid(InspectionActor))
	{
		UpdateActorMeshDelegate.AddDynamic(InspectionActor, &ARAIInspectionActor::UpdateActorMesh);
		RotateActorDelegate.AddDynamic(InspectionActor, &ARAIInspectionActor::AddYawPitch);
		ZoomActorDelegate.AddDynamic(InspectionActor, &ARAIInspectionActor::AddZoom);		
	}
	else
	{
		FTimerHandle TimerHandle_BindInspectionActor;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_BindInspectionActor,
			this,
			&URAIInspectionUI::BindInspectionActor,
			0.1f,
			false
		);
	}
}

FReply URAIInspectionUI::NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent& Event)
{
	if (Event.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bDragging = true; 
		return FReply::Handled(); 
	}
	return FReply::Unhandled();
}

FReply URAIInspectionUI::NativeOnMouseButtonUp(const FGeometry&, const FPointerEvent& Event)
{
	if (Event.GetEffectingButton() == EKeys::LeftMouseButton)
	{ 
		bDragging = false;
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply URAIInspectionUI::NativeOnMouseMove(const FGeometry&, const FPointerEvent& Event)
{
	if (bDragging)
	{
		const FVector2D CursorDelta = Event.GetCursorDelta();
		RotateActorDelegate.Broadcast(CursorDelta.X, -CursorDelta.Y);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply URAIInspectionUI::NativeOnMouseWheel(const FGeometry&, const FPointerEvent& Event)
{
	ZoomActorDelegate.Broadcast(Event.GetWheelDelta());
	return FReply::Handled();
}

void URAIInspectionUI::OpenInspectionUI(const FRAIEvidenceData& InEvidenceData)
{
	RAIPlayerController->EnterInspectionModeIMC();

	UpdateInspectionUI(InEvidenceData);

	SetVisibility(ESlateVisibility::Visible);
}

void URAIInspectionUI::CloseInspectionUI()
{
	//RAIPlayerController->RemoveInspectionModeIMC();
	RAIPlayerController->EnterDefaultModeIMC();

	SetVisibility(ESlateVisibility::Hidden);
}

void URAIInspectionUI::UpdateInspectionUI(const FRAIEvidenceData& InEvidenceData)
{
	UpdateActorMeshDelegate.Broadcast(InEvidenceData.Mesh);

	SetDisplayName(InEvidenceData.DisplayName);
	SetDescription(InEvidenceData.Description);
	SetManualText();
}


void URAIInspectionUI::SetDisplayName(FText InText)
{
	DisplayNameTextBlock->SetText(InText);
}

void URAIInspectionUI::SetDescription(FText InText)
{
	DescriptionTextBlock->SetText(InText);
}

void URAIInspectionUI::SetManualText(int32 AttemptsLeft)
{
	// 1) 현재 활성 매핑에서 키 이름 얻기 시도
	FText ResetKey = GetMappedKeyDisplayName(IA_ResetInspectionMesh);
	FText ExitKey = GetMappedKeyDisplayName(IA_CloseInspectionUI);


	// 2) Unbound라면 한 틱뒤 재시도
	bool bResetReady = !ResetKey.ToString().Equals(TEXT("Unbound"));
	bool bExitReady = !ExitKey.ToString().Equals(TEXT("Unbound"));

	if ((!bResetReady || !bExitReady) && AttemptsLeft > 0)
	{
		// 한 틱만 미뤄서 재시도
		GetWorld()->GetTimerManager().SetTimerForNextTick(
			FTimerDelegate::CreateUObject(this, &URAIInspectionUI::SetManualText, AttemptsLeft - 1));
		return;
	}

	// 3) 최종 텍스트 표시 (Unbound여도 그대로 보여줌)
	ResetManualText = FString::Printf(TEXT("Press [%s] to Reset Preview"), *ResetKey.ToString());
	ExitManualText = FString::Printf(TEXT("Press [%s] to Exit"), *ExitKey.ToString());

	ResetManualTextBlock->SetText(FText::FromString(ResetManualText));
	ExitManualTextBlock->SetText(FText::FromString(ExitManualText));
}

FText URAIInspectionUI::GetMappedKeyDisplayName(const UInputAction* IA) const
{
	if (!IsValid(IA))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: GetMappedKeyDisplayName() failed"), *GetName());
		return FText::FromString(TEXT("-"));
	}

	if (ULocalPlayer* LocalPlayer = RAIPlayerController->GetLocalPlayer())
	{
		if (auto* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			TArray<FKey> MappedKeys = InputSubsystem->QueryKeysMappedToAction(IA);
			if (MappedKeys.Num() > 0)
			{
				return MappedKeys[0].GetDisplayName(false); //true는 디버그용 이름 반환
			}
		}
	}

	//매핑이 없을 경우
	return FText::FromString(TEXT("Unbound"));
}
