#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_ThrowKunai.generated.h"

class AStealthKunai;

UCLASS()
class UGA_ThrowKunai : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_ThrowKunai();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Kunai")
	TSubclassOf<AStealthKunai> KunaiClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Kunai")
	float SpawnForwardOffset = 80.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Kunai")
	float SpawnHeight = 40.f;
};
