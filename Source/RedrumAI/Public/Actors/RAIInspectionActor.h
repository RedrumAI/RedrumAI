// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RAIInspectionActor.generated.h"

class UArrowComponent;
class UPointLightComponent;
class USceneCaptureComponent2D;

UCLASS()
class REDRUMAI_API ARAIInspectionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARAIInspectionActor();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	//컴포넌트 위치는 BP에서 조정
	UPROPERTY(VisibleAnywhere, Category = "RAI")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "RAI")
	TObjectPtr<UArrowComponent> Arrow;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UPointLightComponent> PointLight;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent2D;


	UPROPERTY(EditAnywhere, Category = "RAI")
	float InitialYaw;
	UPROPERTY(EditAnywhere, Category = "RAI")
	float InitialPitch;
	UPROPERTY(EditAnywhere, Category = "RAI")
	float InitialDistance;

	UPROPERTY(EditAnywhere, Category = "RAI")
	float YawSpeed;
	UPROPERTY(EditAnywhere, Category = "RAI")
	float PitchSpeed;
	UPROPERTY(EditAnywhere, Category = "RAI")
	float WheelSpeed;
	UPROPERTY(EditAnywhere, Category = "RAI")
	float MinDistance;
	UPROPERTY(EditAnywhere, Category = "RAI")
	float MaxDistance;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpdateActorMesh(UStaticMesh* InMesh);

	UFUNCTION()
	void AddYawPitch(float DeltaYaw, float DeltaPitch);
	UFUNCTION()
	void AddZoom(float DeltaWheel);

	//UI켜기/닫기, 초기화버튼 등 이벤트 발생하면 호출
	void ResetTransform();
};
