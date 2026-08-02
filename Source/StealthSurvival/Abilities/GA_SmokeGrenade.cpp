#include "GA_SmokeGrenade.h"

#include "World/StealthSmokeCloud.h"

UGA_SmokeGrenade::UGA_SmokeGrenade()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_SmokeGrenade::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	AActor* Avatar = GetAvatarActorFromActorInfo();
	UWorld* World = Avatar != nullptr ? Avatar->GetWorld() : nullptr;
	if (Avatar == nullptr || World == nullptr || SmokeCloudClass == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = Avatar;
	
	World->SpawnActor<AStealthSmokeCloud>(SmokeCloudClass, Avatar->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
