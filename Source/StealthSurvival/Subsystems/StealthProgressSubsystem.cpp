#include "StealthProgressSubsystem.h"

#include "Data/StealthSaveGame.h"
#include "Kismet/GameplayStatics.h"

const FString UStealthProgressSubsystem::SlotName = TEXT("StealthProgress");

void UStealthProgressSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SaveGameObject = Cast<UStealthGameSave>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	}
	
	if (SaveGameObject == nullptr)
	{
		SaveGameObject = Cast<UStealthGameSave>(UGameplayStatics::CreateSaveGameObject(UStealthGameSave::StaticClass()));
	}
	
	if (SaveGameObject != nullptr)
	{
		Difficulty = SaveGameObject->Difficulty;
		BestEscapeTime = SaveGameObject->BestEscapeTime;
		GamesWon = SaveGameObject->GamesWon;
		GamesPlayed = SaveGameObject->GamesPlayed;
	}
}

void UStealthProgressSubsystem::SetDifficulty(EStealthDifficulty NewDifficulty)
{
	Difficulty = NewDifficulty;
	Save();
}

float UStealthProgressSubsystem::GetDetectionMultiplier() const
{
	switch (Difficulty)
	{
	case EStealthDifficulty::Easy : return 0.6f;
	case EStealthDifficulty::Hard: return 1.6f;
	case EStealthDifficulty::Normal:
	default: return 1.f;
	}
}

void UStealthProgressSubsystem::RecordMatchResult(bool bWon, float EscapeTime)
{
	++GamesPlayed;
	if (bWon)
	{
		++GamesWon;
		if (EscapeTime >= 0.f && (BestEscapeTime < 0.f || EscapeTime < BestEscapeTime))
		{
			BestEscapeTime = EscapeTime;
		}
	}
	
	Save();
}

void UStealthProgressSubsystem::Save()
{
	if (SaveGameObject == nullptr)
	{
		return;
	}
	
	SaveGameObject->Difficulty = Difficulty;
	SaveGameObject->BestEscapeTime = BestEscapeTime;
	SaveGameObject->GamesWon = GamesWon;
	SaveGameObject->GamesPlayed = GamesPlayed;
	
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, 0);
}

FText UStealthProgressSubsystem::FormatTime(float Seconds)
{
	const int32 Minutes = FMath::FloorToInt(Seconds / 60.f);
	const int32 Secs = FMath::FloorToInt(Seconds) % 60;
	return FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Secs));
}
