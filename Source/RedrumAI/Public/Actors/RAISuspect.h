// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RAIInteractableInterface.h"
#include "RAIConversationInterface.h"
#include "RAISuspect.generated.h"




UCLASS()
class REDRUMAI_API ARAISuspect : public ACharacter, public IRAIInteractableInterface, public IRAIConversationInterface
{
	GENERATED_BODY()
	
public:
	ARAISuspect();

protected:
    //ACharacter->GetMesh()로 대체
    //UPROPERTY(EditAnywhere, Category = "RAI")
    //TObjectPtr<USkeletalMeshComponent> OriginalMesh;
    UPROPERTY(VisibleAnywhere, Category = "RAI")
    TObjectPtr<USkeletalMeshComponent> OutlineMesh;
    UPROPERTY(VisibleAnywhere, Category = "RAI")
    TObjectPtr<UMaterialInterface> OutlineMaterial;

    UPROPERTY(EditAnywhere, Category = "RAI")
    ERAIInteractType InteractType;

    UPROPERTY(EditAnywhere, Category = "RAI")
    ERAIConversationState ConversationState;
    
    UPROPERTY(VisibleAnywhere, Category = "RAI")
    TObjectPtr<USceneComponent> FrontAnchor;
    UPROPERTY(VisibleAnywhere, Category = "RAI")
    TObjectPtr<USceneComponent> LeftAnchor;
    UPROPERTY(VisibleAnywhere, Category = "RAI")
    TObjectPtr<USceneComponent> RightAnchor;

public:
    virtual void PostInitializeComponents() override;
	virtual void Interacted(AController* InController) override;

	UFUNCTION(BlueprintCallable)
	virtual ERAIConversationState GetConversationState() const override;
	UFUNCTION()
	virtual void CalculateConversationState(FRAIEmotionScore InScore) override;
    UFUNCTION(BlueprintCallable)
	virtual void SetConversationState(ERAIConversationState InState) override;
	
    //캐릭터 Focus시, 관련 함수
    virtual void BeginFocused() override;
    virtual void EndFocused() override;
    virtual void EnableHighlight() override;
    virtual void DisableHighlight() override;

    virtual ERAIInteractType GetInteractType() override;

    virtual ERAIConversationSide GetConversationSide(AController* InController) const override;

    virtual FVector GetFrontAnchorLocation() const override;
    virtual FVector GetLeftAnchorLocation()  const override;
    virtual FVector GetRightAnchorLocation() const override;
    virtual FVector GetHeadWorldLocation()   const override;

    //virtual void OnConversationStarted(AController* InController) override;
    //virtual void OnConversationFinished(AController* InController) override;
};
