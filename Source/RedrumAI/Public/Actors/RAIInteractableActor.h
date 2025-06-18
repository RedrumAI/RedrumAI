// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RAIPlayerInteractableInterface.h"
#include "RAIInteractableActor.generated.h"

class USphereComponent;

UCLASS()
class REDRUMAI_API ARAIInteractableActor : public AActor, public IRAIPlayerInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARAIInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interacted() override;

	void OnBeginOverlapped(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);
	void OnEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	);

	void EnableHighlight();
	void DisableHighlight();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterial> HighlightMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereCollision;
};
