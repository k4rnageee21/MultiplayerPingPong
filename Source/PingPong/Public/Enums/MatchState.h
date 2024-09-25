#pragma once

#include "CoreMinimal.h"
#include "MatchState.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
    WaitingToStart  UMETA(DisplayName = "WaitingToStart"),
    InProgress      UMETA(DisplayName = "InProgress")
};