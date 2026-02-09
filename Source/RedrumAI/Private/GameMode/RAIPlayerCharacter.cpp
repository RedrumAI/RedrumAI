// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Components/SphereComponent.h"
#include "Actors/RAIInteractableInterface.h"


// Sets default values
ARAIPlayerCharacter::ARAIPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractableSphere"));
	InteractableSphere->SetupAttachment(RootComponent); //SphereCollision Radius설정은 BP에서 직접 설정하자.
	InteractableSphere->SetSphereRadius(InteractableDistance);

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called when the game starts or when spawned
void ARAIPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InteractableSphere->OnComponentBeginOverlap.AddDynamic(this, &ARAIPlayerCharacter::OnBeginOverlapped);
	InteractableSphere->OnComponentEndOverlap.AddDynamic(this, &ARAIPlayerCharacter::OnEndOverlapped);
}

// Called every frame
void ARAIPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARAIPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* NewPlayerController = Cast<APlayerController>(NewController))
	{
		// NewPlayerController->PlayerCameraManager->ViewPitchMin = -80.f;
		// NewPlayerController->PlayerCameraManager->ViewPitchMax = 60.f;

		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(NewPlayerController->Player))
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (IsValid(InputMapping))
				{
					InputSystem->AddMappingContext(InputMapping, 0);
				}
			}
		}
	}
}

void ARAIPlayerCharacter::UnPossessed()
{
	Super::UnPossessed();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player))
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (IsValid(InputMapping))
				{
					InputSystem->RemoveMappingContext(InputMapping);
				}
			}
		}
	}
}

// Called to bind functionality to input
void ARAIPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ARAIPlayerCharacter::Move);
	Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ARAIPlayerCharacter::Look);
	Input->BindAction(IA_TriggerInteractableActor, ETriggerEvent::Triggered, this, &ARAIPlayerCharacter::TriggerInteractableActor);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(PlayerController->IsLocalController() && PlayerController->PlayerCameraManager)
		{
			PlayerController->PlayerCameraManager->ViewPitchMin = ViewPitchMinValue;
			PlayerController->PlayerCameraManager->ViewPitchMax = ViewPitchMaxValue;
		}
	}
}

void ARAIPlayerCharacter::Move(const FInputActionInstance& Instance)
{
	FVector2D InputValue = Instance.GetValue().Get<FVector2D>();

	FRotator ControllerRotation = GetControlRotation();
	FRotator YawRotator(0, ControllerRotation.Yaw, 0);
	FVector FowardDirection = FRotationMatrix(YawRotator).GetScaledAxis(EAxis::X); //소스코드는 FVector FowardDirection = YawRotator.Vector();
	FVector RightDirection = FRotationMatrix(YawRotator).GetScaledAxis(EAxis::Y);

	AddMovementInput(FowardDirection, InputValue.Y);
	AddMovementInput(RightDirection, InputValue.X);
}

void ARAIPlayerCharacter::Look(const FInputActionInstance& Instance)
{
	const FVector2D LookValue = Instance.GetValue().Get<FVector2D>();

	if (LookValue.X != 0.f && LookYawScale != 0.f)
	{
		AddControllerYawInput(LookValue.X * LookYawScale);
	}

	if (LookValue.Y != 0.f && LookPitchScale != 0.f)
	{
		AddControllerPitchInput(LookValue.Y * LookPitchScale);
	}
}

void ARAIPlayerCharacter::OnBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IRAIInteractableInterface* InteractableActor = Cast<IRAIInteractableInterface>(OtherActor))
	{
		InteractableActor->EnableHighlight();

		bool bWasEmpty = InteractableActors.IsEmpty();
		InteractableActors.Add(OtherActor);
		if (bWasEmpty && !(InteractableActors.IsEmpty())) //처음 추가됐다면
		{
			GetWorldTimerManager().SetTimer(
				LinetraceTimerHandle,
				this,
				&ARAIPlayerCharacter::TraceInteractableActor,
				0.1f,
				true
			);
		}
	}
}

void ARAIPlayerCharacter::OnEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IRAIInteractableInterface* InteractableActor = Cast<IRAIInteractableInterface>(OtherActor))
	{
		InteractableActor->DisableHighlight();

		InteractableActors.Remove(OtherActor);
		if (InteractableActors.IsEmpty())
		{
			GetWorldTimerManager().ClearTimer(LinetraceTimerHandle);
		}
	}
}

void ARAIPlayerCharacter::TraceInteractableActor()
{
	// Linetrace 시작, 끝 지점 상세설정 필요 (시작: 카메라위치, 끝:카메라ForwardVector*N)
	APlayerController* MyPC = GetController<APlayerController>();
	FVector Start = MyPC->PlayerCameraManager->GetCameraLocation();
	FVector ForwardVector = MyPC->PlayerCameraManager->GetCameraRotation().Vector();
	FVector End = Start + ForwardVector * InteractableDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params); //트레이스 채널을 바꿔 hit이벤트를 더줄일수 있겠다.
	if (bHit)
	{
		//UE_LOG(LogTemp, Log, TEXT("[%s] LineTrace Hit: %s"), *this->GetName(), *(HitResult.GetActor()->GetName()));
	}

	//이전까지 hit하던 액터와 새롭게 hit된 액터가 다르다면 = 조준하는 대상이 변경되었다
	if (CurrentlyFocusedActor != HitResult.GetActor())
	{
		if (IRAIInteractableInterface* OldInteractableActor = Cast<IRAIInteractableInterface>(CurrentlyFocusedActor))
		{
			OldInteractableActor->EndFocused();
		}

		if (IRAIInteractableInterface* NewInteractableActor = Cast<IRAIInteractableInterface>(HitResult.GetActor()))
		{
			NewInteractableActor->BeginFocused();
		}

		//갱신
		CurrentlyFocusedActor = HitResult.GetActor();
	}
}

void ARAIPlayerCharacter::TriggerInteractableActor()
{
	UE_LOG(LogTemp, Warning, TEXT("TriggerInteractableAcotr bHit triggered"));

	// Linetrace 시작, 끝 지점 상세설정 필요 (시작: 카메라위치, 끝:카메라ForwardVector*N)
	APlayerController* MyPC = GetController<APlayerController>();
	FVector Start = MyPC->PlayerCameraManager->GetCameraLocation();
	FVector ForwardVector = MyPC->PlayerCameraManager->GetCameraRotation().Vector();
	FVector End = Start + ForwardVector * InteractableDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params); //트레이스 채널을 바꿔 hit이벤트를 더줄일수 있겠다.
	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerInteractableAcotr Hit!"));

		//Draw Linetrace DebugImage
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 0.2f);
		DrawDebugSphere(GetWorld(), HitResult.Location, 6.f, 12, FColor::Green, false, 0.2f);
		if (IRAIInteractableInterface* InteractableActor = Cast<IRAIInteractableInterface>(HitResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] Hited"), *HitResult.GetActor()->GetName());
			if (InteractableActor->GetInteractType() == ERAIInteractType::Suspect)
			{
				InteractableActor->Interacted(Controller);
			}
			else
			{
				InteractableActor->Interacted();
			}
		}
	}
	else
	{
		//Draw Linetrace DebugImage
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.2f);
		DrawDebugSphere(GetWorld(), End, 6.f, 12, FColor::Green, false, 0.2f);
	}
}
