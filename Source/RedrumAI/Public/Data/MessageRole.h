// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MessageRole.generated.h"

UENUM(BlueprintType)
enum EMessageRole : uint8
{
	Developer	UMETA(DisplayName = "Developer"),
	User		UMETA(DisplayName = "User"),
	Assistant	UMETA(DisplayName = "Assistant")
};