#pragma once

#include "CoreMinimal.h"
#include "StealthAlertState.generated.h"

UENUM()
enum class EStealthAlertState : uint8
{
	Unaware,
	Suspicious,
	Alerted
};
