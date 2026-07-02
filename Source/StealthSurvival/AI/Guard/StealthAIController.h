#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "StealthAIController.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class AStealthAIController : public AAIController
{
	GENERATED_BODY()
public:
	AStealthAIController(const FObjectInitializer& ObjectInitializer);
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
private:
	bool bIsWatchingPlayer = false;
	void SetWatchingPlayer(bool bNewWatching);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Perception")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UFUNCTION()
	void OnTargetForgotten(AActor* Actor);
	
	UFUNCTION()
	void OnAlertReceived(FVector AlertLocation, AActor* AlertInstigator);
};
