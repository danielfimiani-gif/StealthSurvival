#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/StealthGameTypes.h"
#include "StealthMainMenuWidget.generated.h"

class UStealthProgressSubsystem;

UCLASS(abstract)
class UStealthMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="MainMenu")
	void PlayGame();

	UFUNCTION(BlueprintCallable, Category="MainMenu")
	void QuitGame();
	
	UFUNCTION(BlueprintCallable, Category="MainMenu")
	void SetDifficulty(EStealthDifficulty NewDifficulty);
	
	UFUNCTION(BlueprintPure, Category="MainMenu")
	EStealthDifficulty GetDifficulty() const;
	
	UFUNCTION(BlueprintPure, Category="MainMenu")
	FText GetBestTimeText() const;
	
private:
	UStealthProgressSubsystem* GetProgressSubsystem() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="MainMenu")
	FName GameplayLevelName = TEXT("Lvl_Stealth");
};