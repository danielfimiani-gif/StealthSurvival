#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "StealthGameSettings.generated.h"

class UDataTable;

UCLASS(config=Game, defaultconfig, meta=(DisplayName="Stealth Settings Survival"))
class UStealthGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return FName("Stealth Game"); }
	
	UPROPERTY(config, EditAnywhere, Category="Items", meta=(AllowedClasses="/Script/Engine.DataTable"))
	TSoftObjectPtr<UDataTable> ItemTable;
};
