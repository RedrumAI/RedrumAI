// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RAISuspect.h"
#include "Components/CapsuleComponent.h"
#include "GameMode/RAIPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/RAIGameMode.h"
#include "Data/RAIEmotionScore.h"

ARAISuspect::ARAISuspect()
{
	OutlineMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OutlineMesh"));
	OutlineMesh->SetupAttachment(GetMesh());

	//추후 Collision Preset 설정
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OutlineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OutlineMesh->SetCastShadow(false);
	OutlineMesh->SetVisibility(false);

	FrontAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("FrontAnchor"));
	FrontAnchor->SetupAttachment(GetMesh());
	FrontAnchor->bVisualizeComponent = true;
	LeftAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("LeftAnchor"));
	LeftAnchor->SetupAttachment(GetMesh());
	LeftAnchor->bVisualizeComponent = true;
	RightAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("RightAnchor"));
	RightAnchor->SetupAttachment(GetMesh());
	RightAnchor->bVisualizeComponent = true;

	InteractType = ERAIInteractType::Suspect;
	ConversationState = ERAIConversationState::Idle;
}

void ARAISuspect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const FSoftObjectPath OutlineMaterialPath(TEXT("/Script/Engine.Material'/Game/YJ/Actors/EvidenceActor/M_HighlightOutline.M_HighlightOutline'"));
	OutlineMaterial = Cast<UMaterialInterface>(OutlineMaterialPath.TryLoad());

	OutlineMesh->SetSkeletalMesh(GetMesh()->GetSkeletalMeshAsset());
	if (IsValid(OutlineMaterial))
	{
		OutlineMesh->SetMaterial(0, OutlineMaterial);
	}

	//감정점수 bind
	ARAIGameMode* RAIGameMode = Cast<ARAIGameMode>(UGameplayStatics::GetGameMode(this));
	if (RAIGameMode)
	{
		RAIGameMode->SendScoreDelegate.AddDynamic(this, &ARAISuspect::CalculateConversationState);
	}
	
}

void ARAISuspect::Interacted(AController* InController)
{
	ARAIPlayerController* InPC = Cast<ARAIPlayerController>(InController);
	InPC->TryStartConversation(this);
	UE_LOG(LogTemp, Warning, TEXT("[%s] Interacted"), *this->GetName());
}

ERAIConversationState ARAISuspect::GetConversationState() const
{
	return ConversationState;
}

void ARAISuspect::CalculateConversationState(FRAIEmotionScore InScore)
{
	//V = (Love + Joy) - (Anger + Fear + Sadness)
	//V >= +0.20 : 긍정 애니메이션
	//V <= -0.20 : 부정 애니메이션
	//그 사이 : 평범 애니메이션

	float sum = (InScore.Love + InScore.Joy) - (InScore.Anger + InScore.Fear + InScore.Sadness);
	if (sum >= 0.2)
	{
		SetConversationState(ERAIConversationState::Positive);
	}
	else if (sum <= -0.2)
	{
		SetConversationState(ERAIConversationState::Negative);
	}
	else
	{
		SetConversationState(ERAIConversationState::Normal);
	}
}

void ARAISuspect::SetConversationState(ERAIConversationState InState)
{
	ConversationState = InState;
}

void ARAISuspect::BeginFocused()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Begin Focused"), *this->GetName());
}

void ARAISuspect::EndFocused()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] End Focused"), *this->GetName());
}

void ARAISuspect::EnableHighlight()
{
	//UE_LOG(LogTemp, Warning, TEXT("EnableHighlight On"));
	//OutlineMesh->SetVisibility(false);
}

void ARAISuspect::DisableHighlight()
{
	//UE_LOG(LogTemp, Warning, TEXT("DisableHighlight"));
	//OutlineMesh->SetVisibility(false);
}

ERAIInteractType ARAISuspect::GetInteractType()
{
	return InteractType;
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
	return FrontAnchor ? FrontAnchor->GetComponentLocation() : GetActorLocation();
}

FVector ARAISuspect::GetLeftAnchorLocation() const
{
	return LeftAnchor ? LeftAnchor->GetComponentLocation() : GetActorLocation();
}

FVector ARAISuspect::GetRightAnchorLocation() const
{
	return RightAnchor ? RightAnchor->GetComponentLocation() : GetActorLocation();
}

FVector ARAISuspect::GetHeadWorldLocation() const
{
	if (GetMesh()->DoesSocketExist(TEXT("Head")))
	{
		return GetMesh()->GetSocketLocation(TEXT("Head"));
	}
	return GetActorLocation(); // 예비값
}
