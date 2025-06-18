// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RAIPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

// Sets default values
ARAIPlayerCharacter::ARAIPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARAIPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

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

// Called every frame
void ARAIPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARAIPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ARAIPlayerCharacter::Move);
}

void ARAIPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* NewPlayerController = Cast<APlayerController>(NewController))
	{
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

