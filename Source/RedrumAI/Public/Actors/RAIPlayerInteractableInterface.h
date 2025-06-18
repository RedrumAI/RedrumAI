#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RAIPlayerInteractableInterface.generated.h"

/*
이 클래스는 수정할 필요가 없습니다.
리플렉션 시스템 비저빌리티에 대한 빈 클래스입니다.
UINTERFACE 매크로를 사용합니다.
UInterface에서 상속합니다.
*/
UINTERFACE(MinimalAPI, Blueprintable)
class URAIPlayerInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/* 실제 인터페이스 선언. */
class IRAIPlayerInteractableInterface
{
	GENERATED_BODY()
	
public:
	// 여기에 인터페이스 함수 선언을 추가합니다
	virtual void Interacted();
};