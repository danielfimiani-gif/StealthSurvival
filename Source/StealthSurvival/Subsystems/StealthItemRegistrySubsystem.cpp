#include "StealthItemRegistrySubsystem.h"

#include "Data/StealthGameSettings.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "Engine/Texture2D.h"

void UStealthItemRegistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const UStealthGameSettings* Settings = GetDefault<UStealthGameSettings>();
	if (Settings != nullptr && !Settings->ItemTable.IsNull())
	{
		ItemTable = Settings->ItemTable.LoadSynchronous();
	}
	
	if (ItemTable != nullptr)
	{
		StreamItemIcons();
	}
}

void UStealthItemRegistrySubsystem::Deinitialize()
{
	if (IconStreamHandle.IsValid())
	{
		IconStreamHandle->CancelHandle();
		IconStreamHandle.Reset();
	}
	
	Super::Deinitialize();
}

void UStealthItemRegistrySubsystem::StreamItemIcons()
{
	TArray<FSoftObjectPath> IconPaths;
	
	static const FString Context(TEXT("StealthItemRegistry_Stream"));
	TArray<FStealthItemData*> Rows;
	ItemTable->GetAllRows<FStealthItemData>(Context, Rows);
	for (const FStealthItemData* Row : Rows)
	{
		if (Row != nullptr && !Row->Icon.IsNull())
		{
			IconPaths.Add(Row->Icon.ToSoftObjectPath());
		}
	}
	
	if (IconPaths.Num() > 0)
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		IconStreamHandle = Streamable.RequestAsyncLoad(IconPaths);
	}
}

bool UStealthItemRegistrySubsystem::GetItemData(FName ItemId, FStealthItemData& OutData) const
{
	if (ItemTable == nullptr || ItemId.IsNone())
	{
		return  false;
	}
		
	static  const FString Context(TEXT("StealthItemRegistry_Lookup"));
	if (const FStealthItemData* Row = ItemTable->FindRow<FStealthItemData>(ItemId, Context, false))
	{
		OutData = *Row;
		return true;
	}
	
	return false;
}

FText UStealthItemRegistrySubsystem::GetObjectiveText(FName ItemId) const
{
	FStealthItemData Data;
	return GetItemData(ItemId, Data) ? Data.ObjectiveText : FText::GetEmpty();
}

FText UStealthItemRegistrySubsystem::GetDisplayName(FName ItemId) const
{
	FStealthItemData Data;
	return GetItemData(ItemId, Data) ? Data.DisplayName : FText::GetEmpty();
}

UTexture2D* UStealthItemRegistrySubsystem::GetItemIcon(FName ItemId) const
{
	FStealthItemData Data;
	if (!GetItemData(ItemId, Data) || Data.Icon.IsNull())
	{
		return nullptr;
	}
	
	return Data.Icon.Get();
}
