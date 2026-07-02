#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StealthSurvivalGameState.generated.h"

UENUM(BlueprintType)
enum class EStealthMatchState : uint8
{
	Playing,
	Won,
	Lost
};

UCLASS()
class AStealthSurvivalGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Match")	
	float GetDetectionLevel() const { return DetectionLevel; }
	void SetDetectionLevel(float Level) { DetectionLevel = FMath::Clamp(Level, 0.f, 1.f); }
	
	UFUNCTION(BlueprintPure, Category="Match")
	bool HasObjective() const { return bHasObjective; }
	
	UFUNCTION(BlueprintPure, Category="Match")
	EStealthMatchState GetMatchState() const { return MatchState; }
	
	void SetHasObjective(bool value) { bHasObjective = value; }
	void SetMatchState(EStealthMatchState value) { MatchState = value; }
	
	UFUNCTION(BlueprintPure, Category="Match")
	bool HasKey(FName KeyId) const { return CollectedKeys.Contains(KeyId); }
	
	void AddKey(FName KeyId) { CollectedKeys.Add(KeyId); }
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Match")
	float DetectionLevel = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category="Match")
	EStealthMatchState MatchState = EStealthMatchState::Playing;
	
	UPROPERTY(BlueprintReadOnly, Category="Match")
	bool bHasObjective = false;
	
	UPROPERTY()
	TSet<FName> CollectedKeys;
};
