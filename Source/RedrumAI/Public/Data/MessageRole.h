// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MessageRole.generated.h"

UENUM(BlueprintType)
enum EMessageRole : uint8
{
	developer	UMETA(DisplayName = "developer"),
	user		UMETA(DisplayName = "user"),
	assistant	UMETA(DisplayName = "assistant")
};