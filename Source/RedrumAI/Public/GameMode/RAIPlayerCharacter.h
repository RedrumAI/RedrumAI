// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "RAIPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USphereComponent;

UCLASS()
class REDRUMAI_API ARAIPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARAIPlayerCharacter();

protected:
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputMappingContext> InputMapping;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_TriggerInteractableActor;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAI")
	TObjectPtr<USphereComponent> InteractableSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAI")
	float InteractableDistance = 500.f; //BP에서 상세설정
	UPROPERTY(EditAnywhere)
	TSet<AActor*> InteractableActors;

	AActor* CurrentlyFocusedActor = nullptr;
	FTimerHandle LinetraceTimerHandle;

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnBeginOverlapped(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	void OnEndOverlapped(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	void TraceInteractableActor();
	void TriggerInteractableActor();

	void Move(const FInputActionInstance& Instance);	
};
