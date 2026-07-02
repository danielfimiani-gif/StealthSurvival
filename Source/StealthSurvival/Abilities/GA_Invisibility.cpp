#include "GA_Invisibility.h"

#include "TimerManager.h"
#include "Player/StealthSurvivalCharacter.h"

UGA_Invisibility::UGA_Invisibility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Invisibility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (AStealthSurvivalCharacter* Character = Cast<AStealthSurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->SetInvisible(true);
	}
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(DurationTimerHandle, this, &UGA_Invisibility::OnDurationExpired, MaxDuration, false);
	}
}

void UGA_Invisibility::OnDurationExpired()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Invisibility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DurationTimerHandle);
	}
	
	if (ActorInfo != nullptr)
	{
		if (AStealthSurvivalCharacter* Character = Cast<AStealthSurvivalCharacter>(ActorInfo->AvatarActor.Get()))
		{
			Character->SetInvisible(false);
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
