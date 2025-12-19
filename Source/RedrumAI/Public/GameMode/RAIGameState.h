// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RAIGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateEvidenceRowsDelegate);

struct FRAIEvidenceData;

UCLASS()
class REDRUMAI_API ARAIGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	/*
	UPROPERTY(EditDefaultsOnly, Category = "RAI")
	TObjectPtr<UDataTable> EvidenceDataTable;
	*/
	//GameMode에서 게임시작하면 레벨데이터를 GS에 넘기고 UI가 이걸보고 반영
	UPROPERTY(EditDefaultsOnly, Category = "RAI") //지금 레벨에 필요한 데이터(용의자, 정답)를 들고있어야한다.
	TObjectPtr<UDataTable> LevelDataTable;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TArray<FName> EvidenceRows;


public:
	virtual void PostInitializeComponents() override;

	void AddEvidence(FName InRowName);
	void RemoveEvidence(FName InRowName);

	TArray<FName> GetEvidenceRows() const;
	const FRAIEvidenceData* FindEvidenceData(FName RowName) const;

	UPROPERTY()
	FOnUpdateEvidenceRowsDelegate UpdateEvidenceRowsDelegate;
};
