// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAISuspect.h"
#include "GameMode/RAIPlayerController.h"

ARAISuspect::ARAISuspect()
{
	FrontAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("FrontAnchor"));
	FrontAnchor->SetupAttachment(OriginalMesh);
	LeftAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("LeftAnchor"));
	LeftAnchor->SetupAttachment(OriginalMesh);
	RightAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("RightAnchor"));
	RightAnchor->SetupAttachment(OriginalMesh);

	InteractType = ERAIInteractType::Suspect;
}

void ARAISuspect::Interacted(AController* InController)
{
	ARAIPlayerController* InPC = Cast<ARAIPlayerController>(InController);
	InPC->SetTalkingState(true);
}

ERAIConversationSide ARAISuspect::GetConversationSide(AController* InController) const
{
	const APlayerController* PlayerController = Cast<APlayerController>(InController);
	ensure(PlayerController);

	// 플레이어와 용의자 위치
	const FVector PawnLocation = PlayerController->GetPawn()->GetActorLocation();
	const FVector SuspectLocation = GetActorLocation();

	// 1. 용의자 기준 "정면 시야각" 안에 카메라가 있는지 판정
	FVector SuspectForwardVector = GetActorForwardVector();
	SuspectForwardVector.Z = 0.f;
	SuspectForwardVector.Normalize();

	FVector SuspectToPawnVector = PawnLocation - SuspectLocation;
	SuspectToPawnVector.Z = 0.f;
	SuspectToPawnVector.Normalize();

	const float DotForward = FVector::DotProduct(SuspectForwardVector, SuspectToPawnVector);

	// VisionHalfAngle: 용의자 전방 시야각의 '절반 각도', 인터페이스 내부에 존재
	const float CosVisionHalfAngle = FMath::Cos(FMath::DegreesToRadians(VisionHalfAngle));

	if (DotForward >= CosVisionHalfAngle)
	{
		return ERAIConversationSide::Front;
	}
	else
	{
		// 2. 정면이 아니라면, 용의자 기준 좌/우 판정
		FVector SuspectRightVector = GetActorRightVector();
		SuspectRightVector.Z = 0.f;
		SuspectRightVector.Normalize();

		const float DotRight = FVector::DotProduct(SuspectRightVector, SuspectToPawnVector);

		return (DotRight >= 0.f) ? ERAIConversationSide::Right : ERAIConversationSide::Left;
	}
}

FVector ARAISuspect::GetFrontAnchorLocation() const
{
	return FVector();
}

FVector ARAISuspect::GetLeftAnchorLocation() const
{
	return FVector();
}

FVector ARAISuspect::GetRightAnchorLocation() const
{
	return FVector();
}

FVector ARAISuspect::GetHeadWorldLocation() const
{
	return FVector();
}
