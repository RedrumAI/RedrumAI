// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RAIGameMode.generated.h"

class ARAIHttpManager;
class ARAIChatManager;

UCLASS()
class REDRUMAI_API ARAIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARAIGameMode();
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void BindHM2CM();

	void tmpRequestNLP();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<ARAIHttpManager> HttpManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<ARAIChatManager> ChatManager;

	FTimerHandle TimerHandle_BindHM;

};
