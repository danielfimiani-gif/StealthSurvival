#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StealthHUDWidget.generated.h"

class AStealthSurvivalGameState;
class AStealthSurvivalCharacter;
class UStealthItemRegistrySubsystem;

UCLASS(Abstract)
class UStealthHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="HUD")
	float GetDetectionLevel() const;
	
	UFUNCTION(BlueprintCallable, Category="HUD")
	FText GetObjectiveText() const;
	
	UFUNCTION(BlueprintCallable, Category="HUD")
	bool IsPlayerConcealed() const;

	UFUNCTION(BlueprintPure, Category="HUD")
	float GetInvisibilityCooldownPercent() const;

	UFUNCTION(BlueprintPure, Category="HUD")
	bool IsPlayerInvisible() const;

	UFUNCTION(BlueprintPure, Category="HUD")
	FText GetMatchTimeText() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	FText CardObjectiveText = FText::FromString(TEXT("Obtené la tarjeta de acceso"));

	UPROPERTY(EditDefaultsOnly, Category="HUD")
	FText StealObjectiveText = FText::FromString(TEXT("Robá los planos"));
	
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	FText FindCodeObjectiveText = FText::FromString(TEXT("Encontrá el código de acceso"));

	UPROPERTY(EditDefaultsOnly, Category="HUD")
	FText EscapeObjectiveText = FText::FromString(TEXT("Escapá a la zona segura"));

	UPROPERTY(EditDefaultsOnly, Category="HUD")
	FName AccessCardId = TEXT("AccessCard");

	UPROPERTY(EditDefaultsOnly, Category="HUD")
	FName EscapeCodeId = TEXT("EscapeCode");
	
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	FName ObjectiveItemId = TEXT("Plans");

	UPROPERTY(EditDefaultsOnly, Category="HUD")
	FName EscapeStepId = TEXT("Escape");
	
private:
	AStealthSurvivalGameState* GetStealthGameState() const;
	AStealthSurvivalCharacter* GetStealthPlayer() const;
	const UStealthItemRegistrySubsystem* GetItemRegistry() const;
};
