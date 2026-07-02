#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "StealthAlertSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAlertRaised, FVector, AlertLocation, AActor*, AlertInstigator);

UCLASS()
class UStealthAlertSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Alert")
	FOnAlertRaised OnAlertRaised;
	
	UFUNCTION(BlueprintCallable, Category="Alert")
	void RaiseAlert(FVector AlertLocation, AActor* AlertInstigator);
};