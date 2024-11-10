#pragma once

#include "CoreMinimal.h"
#include "StateEnum.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8 
{
    Idle = 0,
    Walk,
    Run,
    InAir
};