#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StealthMainMenuWidget.generated.h"

UCLASS(abstract)
class UStealthMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="MainMenu")
	void PlayGame();

	UFUNCTION(BlueprintCallable, Category="MainMenu")
	void QuitGame();

protected:
	UPROPERTY(EditDefaultsOnly, Category="MainMenu")
	FName GameplayLevelName = TEXT("Lvl_Stealth");
};