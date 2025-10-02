#pragma once

#include "RAIInteractType.generated.h"

UENUM(BlueprintType)
enum class ERAIInteractType : uint8
{
    Unknown     UMETA(DisplayName = "Unknown"),
    Collectable UMETA(DisplayName = "Collectable"),
    Fixture     UMETA(DisplayName = "Fixture"),
    Suspect     UMETA(DisplayName = "Suspect")   
};