#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthSurvivalGameMode.generated.h"

class AStealthSurvivalGameState;

UCLASS(abstract)
class AStealthSurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStealthSurvivalGameMode();
	
	virtual void Tick(float DeltaSecondsDeltaTime) override;
	
	void AddWatcher();
	void RemoveWatcher();
	
	void NotifyObjectiveStolen();
	void NotifyPlayerReachedExit();
	void NotifyKeyCollected(FName KeyId);
	
protected:
	void TriggerWin();
	void TriggerLose();
	
	AStealthSurvivalGameState* GetStealthSurvivalGameState() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Detection", meta=(ClampMin = "0"))
	float DetectionRiseRate = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Detection", meta=(ClampMin = "0"))
	float DetectionFallRate = 0.2f;
	
	int32 ActiveWatchers = 0;
};



