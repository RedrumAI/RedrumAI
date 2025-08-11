#pragma once

#include "RAIEvidenceType.generated.h"

UENUM(BlueprintType)
enum class ERAIEvidenceType : uint8
{
    Collectible UMETA(DisplayName = "Collectible"),
    InspectOnly UMETA(DisplayName = "Inspect Only")
};