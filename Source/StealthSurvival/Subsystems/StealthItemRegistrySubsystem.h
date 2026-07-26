#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/StealthItemData.h"
#include "StealthItemRegistrySubsystem.generated.h"

class UDataTable;
class UTexture2D;
struct FStreamableHandle;

UCLASS()
class UStealthItemRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category="Items")
	bool GetItemData(FName ItemId, FStealthItemData& OutData) const;
	
	UFUNCTION(BlueprintPure, Category="Items")
	FText GetObjectiveText(FName ItemId) const;
	
	UFUNCTION(BlueprintPure, Category="Items")
	FText GetDisplayName(FName ItemId) const;
	
	UFUNCTION(BlueprintPure, Category="Items")
	UTexture2D* GetItemIcon(FName ItemId) const;
	
private:
	void StreamItemIcons();
	
	UPROPERTY()
	TObjectPtr<UDataTable> ItemTable;
	
	TSharedPtr<FStreamableHandle> IconStreamHandle;
};
