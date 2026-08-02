#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "StealthAlertState.h"
#include "Data/StealthGuardConfig.h"
#include "StealthGuardCharacter.generated.h"

class UBehaviorTree;
class UStaticMeshComponent;
class UWidgetComponent;

UCLASS(abstract)
class AStealthGuardCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}
	
	const TArray<TObjectPtr<AActor>>& GetPatrolPoints() const { return PatrolPoints; }
	
	void Die();
	
	void Stun(float Duration);
	
	UFUNCTION(BlueprintPure, Category="AI")
	bool IsStunned() const { return bIsStunned; }

	void SetTakedownAvailable(bool bAvailable);

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override { TeamId = NewTeamID; }
	
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	
	UFUNCTION(BlueprintPure, Category = "AI")
	bool IsDead() const { return bIsDead; }
	
	void SetAlertState(EStealthAlertState NewState);

	float GetSightRange() const { return GuardConfig ? GuardConfig->SightRange : SightRange; }
	float GetSightHalfAngle() const { return GuardConfig ? GuardConfig->SightHalfAngle : SightHalfAngle; }
	float GetPatrolSpeed() const { return GuardConfig ? GuardConfig->PatrolSpeed : PatrolSpeed; }
	float GetChaseSpeed() const { return GuardConfig ? GuardConfig->ChaseSpeed : ChaseSpeed; }
	
	FLinearColor GetAlertColor(EStealthAlertState State) const;
	
protected:
	AStealthGuardCharacter();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;
	
	bool bIsDead = false;
	
	void EndStun();
	bool bIsStunned = false;
	FTimerHandle StunTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	TObjectPtr<UStealthGuardConfig> GuardConfig;
	
	UPROPERTY(EditDefaultsOnly, Category="AI", meta=(ClampMin="0"))
	float DeathLifespan = 5.f;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Patrol")
	TArray<TObjectPtr<AActor>> PatrolPoints;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vision")
	TObjectPtr<UStaticMeshComponent> VisionConeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UWidgetComponent> TakedownPrompt;

	UPROPERTY(EditDefaultsOnly, Category="Vision")
	FLinearColor UnawareColor = FLinearColor(0.1f, 1.f, 0.2f);

	UPROPERTY(EditDefaultsOnly, Category="Vision")
	FLinearColor SuspiciousColor = FLinearColor(1.f, 0.8f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category="Vision")
	FLinearColor AlertedColor = FLinearColor(1.f, 0.1f, 0.1f);

	UPROPERTY(BlueprintReadOnly, Category="Vision")
	EStealthAlertState AlertState = EStealthAlertState::Unaware;

	UPROPERTY(EditDefaultsOnly, Category="Vision", meta=(ClampMin="0"))
	float SightRange = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category="Vision", meta=(ClampMin="0", ClampMax="90"))
	float SightHalfAngle = 45.f;

	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ClampMin="0"))
	float PatrolSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ClampMin="0"))
	float ChaseSpeed = 450.f;

private:
	FGenericTeamId TeamId;
};
