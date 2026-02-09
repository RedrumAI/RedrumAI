// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RAIConversationInterface.generated.h"

UENUM(BlueprintType)
enum class ERAIConversationSide : uint8
{
	Front  UMETA(DisplayName = "Front"),
	Left   UMETA(DisplayName = "Left"),
	Right  UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
enum class ERAIConversationState : uint8
{
	Idle  UMETA(DisplayName = "Idle"),
	Normal  UMETA(DisplayName = "Normal"),
	Agree  UMETA(DisplayName = "Agree"),
	Disagree   UMETA(DisplayName = "Left")
};

UINTERFACE(MinimalAPI)
class URAIConversationInterface : public UInterface
{
	GENERATED_BODY()
};

class REDRUMAI_API IRAIConversationInterface
{
	GENERATED_BODY()

public:
    float VisionHalfAngle = 50.f;

    // 플레이어 컨트롤러를 기준으로, 이 타겟 기준 방향을 계산
    virtual ERAIConversationSide GetConversationSide(AController* InController) const = 0;

    // 각 앵커 위치
    virtual FVector GetFrontAnchorLocation() const = 0;
    virtual FVector GetLeftAnchorLocation()  const = 0;
    virtual FVector GetRightAnchorLocation() const = 0;

    // 카메라가 바라볼 위치(머리/목 등)
    virtual FVector GetHeadWorldLocation() const = 0;

    // (선택) 대화 시작/종료 시 훅
    // virtual void OnConversationStarted(AController* InController) {}
    // virtual void OnConversationFinished(AController* InController) {}
	
};
