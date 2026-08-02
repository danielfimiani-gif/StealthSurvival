#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_SmokeGrenade.generated.h"

class AStealthSmokeCloud;

UCLASS()
class UGA_SmokeGrenade : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_SmokeGrenade();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Smoke")
	TSubclassOf<AStealthSmokeCloud> SmokeCloudClass;
};
