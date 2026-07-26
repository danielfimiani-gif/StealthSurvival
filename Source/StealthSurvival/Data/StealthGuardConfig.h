#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StealthGuardConfig.generated.h"

UCLASS(BlueprintType)
class UStealthGuardConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(ClampMin="0"))
	float PatrolSpeed = 150.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(ClampMin="0"))
	float ChaseSpeed = 450.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vision", meta=(ClampMin="0"))
	float SightRange = 1500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vision", meta=(ClampMin="0", ClampMax="90"))
	float SightHalfAngle = 45.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vision")
	FLinearColor UnawareColor = FLinearColor(0.1f, 1.f, 0.2f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vision")
	FLinearColor SuspiciousColor = FLinearColor(1.f, 0.8f, 0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vision")
	FLinearColor AlertedColor = FLinearColor(1.f, 0.1f, 0.1f);
};
