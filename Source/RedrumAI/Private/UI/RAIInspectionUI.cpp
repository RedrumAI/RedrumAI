// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RAIInspectionUI.h"
#include "GameMode/RAIPlayerController.h"
#include "Actors/RAIInspectionActor.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void URAIInspectionUI::NativeConstruct()
{
	DisplayNameTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_DisplayName")));
	DescriptionTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Description")));

	DisplayNameTextBlock->SetAutoWrapText(true);
	DescriptionTextBlock->SetAutoWrapText(true);

	BindInspectionActor();	
}

void URAIInspectionUI::BindInspectionActor()
{
	ARAIPlayerController* RAIPlayerController = Cast<ARAIPlayerController>(GetOwningPlayer());
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

void URAIInspectionUI::UpdateInspectionUI(const FRAIEvidenceData& InEvidenceData)
{
	UpdateActorMeshDelegate.Broadcast(InEvidenceData.Mesh);

	SetDisplayName(InEvidenceData.DisplayName);
	SetDescription(InEvidenceData.Description);

	ShowInspectionUI();	
}

void URAIInspectionUI::SetDisplayName(FText InText)
{
	DisplayNameTextBlock->SetText(InText);
}

void URAIInspectionUI::SetDescription(FText InText)
{
	DescriptionTextBlock->SetText(InText);
}

void URAIInspectionUI::ShowInspectionUI()
{
	SetVisibility(ESlateVisibility::Visible);
}

void URAIInspectionUI::HideInspectionUI()
{
	SetVisibility(ESlateVisibility::Hidden);
}