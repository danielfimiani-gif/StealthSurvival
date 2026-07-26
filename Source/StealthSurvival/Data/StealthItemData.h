#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StealthItemData.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FStealthItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText ObjectiveText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta=(MultiLine=true))
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSoftObjectPtr<UTexture2D> Icon;
};
