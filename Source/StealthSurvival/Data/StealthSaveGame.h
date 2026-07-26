#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Data/StealthGameTypes.h"
#include "StealthSaveGame.generated.h"

UCLASS()
class UStealthGameSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	EStealthDifficulty Difficulty = EStealthDifficulty::Normal;
	
	UPROPERTY()
	float BestEscapeTime = -1.f;
	
	UPROPERTY()
	int32 GamesWon = 0;
	
	UPROPERTY()
	int32 GamesPlayed = 0;
};
