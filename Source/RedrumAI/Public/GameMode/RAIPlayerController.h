// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RAIPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveSlideInventoryDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateTalkingStateDelegate, bool, bIsTalking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseInspectionUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetInspectionMeshDelegate);

UENUM()
enum class ERAIConversationApproachState : uint8
{
	None,
	ToSideAnchor,
	ToFrontAnchor
};

class ARAIGameMode;
class URAIStageHUDWidget;
class UInputMappingContext;
class UInputAction;
class ARAIInspectionActor;
class IRAIConversationInterface;
class URAILobbyUI;
class ATargetPoint;

UCLASS()
class REDRUMAI_API ARAIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARAIPlayerController();

protected:

	UPROPERTY()
	TObjectPtr<ARAIGameMode> RAIGameMode;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TSubclassOf<URAILobbyUI> LobbyUIClass;
	UPROPERTY()
	TObjectPtr<URAILobbyUI> LobbyUI;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TSubclassOf< URAIStageHUDWidget> StageHUDClass;
	UPROPERTY()
	TObjectPtr<URAIStageHUDWidget> StageHUD;

	UPROPERTY()
	TObjectPtr<ATargetPoint> StageTargetPoint;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TSubclassOf<ARAIInspectionActor> BP_InspectionActor;
	UPROPERTY()
	TObjectPtr<ARAIInspectionActor> InspectionActor; //위치는 BP_PC에서 조정

	//모드전환용 IMC배열(자동화)
	TArray<UInputMappingContext*> IMCArray;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputMappingContext> IMC_DefaultMode;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_ToggleMouseCursor;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_CloseLastUI;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_MoveSlideInventory;

	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputMappingContext> IMC_InspectionMode;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_CloseInspectionUI;
	UPROPERTY(EditAnywhere, Category = "RAI")
	TObjectPtr<UInputAction> IA_ResetInspectionUI;

	bool bIsTalking = false;

	ERAIConversationApproachState ApproachState = ERAIConversationApproachState::None;
	IRAIConversationInterface* CurrentSuspect = nullptr;
	FVector ApproachStartLocation;
	FVector ApproachTargetLocation;
	float   ApproachElapsedTime = 0.f;
	float   ApproachDuration = 0.4f; // 0.4초 정도

public:
	UPROPERTY()
	FOnMoveSlideInventoryDelegate MoveSlideInventoryDelegate;
	UPROPERTY()
	FOnUpdateTalkingStateDelegate UpdateTalkingStateDelegate;
	UPROPERTY()
	FOnCloseInspectionUIDelegate CloseInspectionUIDelegate;
	UPROPERTY()
	FOnResetInspectionMeshDelegate ResetInspectionMeshDelegate;

public:
	virtual void BeginPlay() override;

	void SetupLevelByRowName(FName InRowName);

	void FindStageTargetPoint();
	UFUNCTION()
	void StartLevel();
	void PlayIntroSequence();
	UFUNCTION()
	void SetupStageAfterIntro();

	virtual void PlayerTick(float DeltaTime) override;

	void BindGM();
	void BindGS();
	void BindHUD();
	virtual void SetupInputComponent() override;

	void EnterDefaultModeIMC();
	void ToggleMouseCursor();
	void CloseLastUI();
	void MoveSlideInventory();

	void EnterInspectionModeIMC();
	void CloseInspectionUI();
	void ResetInspectionMesh();

	UFUNCTION()
	void SetAIChat(FString String);
	UFUNCTION()
	void AddChatLogUI(FString InRole, FString InMessage);

	void TryStartConversation(IRAIConversationInterface* Suspect);
	void StartApproachToFront(IRAIConversationInterface* Suspect);
	void StartApproachToSide(IRAIConversationInterface* Suspect, bool bUseRight);
	void TickConversationApproach(float DeltaSeconds);
	bool GetTalkingState() const;
	void SetTalkingState(bool InBool);
	UFUNCTION()
	void SwitchTalkingMode(bool InBool);

	void AskSuspect(FText Text);

	void UseEvidence(FName InRowName);

	const ARAIInspectionActor* GetInspectionActor();

};
