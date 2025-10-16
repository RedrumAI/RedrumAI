#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/RAIInteractType.h"
#include "RAIInteractableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class URAIInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/* 실제 인터페이스 선언. */
class IRAIInteractableInterface
{
	GENERATED_BODY()
	
public:
	// 여기에 인터페이스 함수 선언을 추가합니다
	virtual void Interacted();
	virtual void Interacted(AController* InController);
	virtual void EnableHighlight();
	virtual void DisableHighlight();
	virtual void BeginFocused();
	virtual void EndFocused();

	virtual ERAIInteractType GetInteractType();
};