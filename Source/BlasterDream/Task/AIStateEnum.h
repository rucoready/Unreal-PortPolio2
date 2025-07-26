
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAIStateEnum : uint8
{
    OutOfSight  UMETA(DisplayName = "OutOfSight"),
    Chase   UMETA(DisplayName = "Chase"),
    Attack  UMETA(DisplayName = "Attack"),
    Die     UMETA(DisplayName = "Die"),
    Damaged     UMETA(DisplayName = "Damaged")
};
