#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StealthEndScreenWidget.generated.h"

class AStealthSurvivalGameState;
class UStealthProgressSubsystem;

UCLASS(abstract)
class UStealthEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="EndScreen")
	bool IsVictory() const;

	UFUNCTION(BlueprintCallable, Category="EndScreen")
	void RestartLevel();

	UFUNCTION(BlueprintCallable, Category="EndScreen")
	void OpenMainMenu();
	
	UFUNCTION(BlueprintPure, Category="EndScreen")
	FText GetResultTimeText() const;
	
	UFUNCTION(BlueprintPure, Category="EndScreen")
	FText GetBestTimeText() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="EndScreen")
	FName MainMenuLevelName = TEXT("Lvl_MainMenu");

private:
	AStealthSurvivalGameState* GetStealthGameState() const;
	UStealthProgressSubsystem* GetProgressSubsystem() const;
};