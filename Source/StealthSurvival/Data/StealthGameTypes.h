#pragma once

#include "CoreMinimal.h"
#include "StealthGameTypes.generated.h"

UENUM(BlueprintType)
enum class EStealthDifficulty : uint8
{
	Easy UMETA(DisplayName = "Fácil"),
	Normal UMETA(DisplayName = "Normal"),
	Hard UMETA(DisplayName = "Difícil"),
};
