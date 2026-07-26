#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/StealthGameTypes.h"
#include "StealthProgressSubsystem.generated.h"

class UStealthGameSave;

UCLASS()
class UStealthProgressSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable, Category="Progress")
	void SetDifficulty(EStealthDifficulty NewDifficulty);
	
	UFUNCTION(BlueprintPure, Category="Progress")
	EStealthDifficulty GetDifficulty() const { return Difficulty;}
	
	UFUNCTION(BlueprintPure, Category="Progress")
	float GetDetectionMultiplier() const;
	
	UFUNCTION(BlueprintCallable, Category="Progress")
	void RecordMatchResult(bool bWon, float EscapeTime);
	
	UFUNCTION(BlueprintPure, Category="Progress")
	float GetBestEscapeTime() const { return BestEscapeTime; }
	
	UFUNCTION(BlueprintPure, Category="Progress")
	bool HasBestTime() const { return BestEscapeTime > 0.0f; }
	
	UFUNCTION(BlueprintPure, Category="Progress")
	int32 GetGamesWon() const { return GamesWon; }
	
	UFUNCTION(BlueprintPure, Category="Progress")
	int32 GetGamesPlayed() const { return GamesPlayed; }
	
	UFUNCTION(BlueprintPure, Category="Progress")
	static FText FormatTime(float Seconds);
	
private:
	void Save();
	
	UPROPERTY()
	TObjectPtr<UStealthGameSave> SaveGameObject;
	
	EStealthDifficulty Difficulty = EStealthDifficulty::Normal;
	float BestEscapeTime = -1.0f;
	int32 GamesWon = 0;
	int32 GamesPlayed = 0;
	
	static const FString SlotName;
};
