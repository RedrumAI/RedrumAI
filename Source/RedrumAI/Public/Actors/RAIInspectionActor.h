// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RAIInspectionActor.generated.h"

class UPointLightComponent;
class USceneCaptureComponent2D;

UCLASS()
class REDRUMAI_API ARAIInspectionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARAIInspectionActor();

protected:
	virtual void BeginPlay() override;

	//컴포넌트 위치는 BP에서 조정
	UPROPERTY(VisibleAnywhere, Category = "RAI")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UPointLightComponent> PointLight;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent2D;

public:	
	virtual void Tick(float DeltaTime) override;

};
