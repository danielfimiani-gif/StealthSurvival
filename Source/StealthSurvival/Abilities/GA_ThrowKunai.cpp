#include "GA_ThrowKunai.h"

#include "Items/StealthKunai.h"
#include "GameFramework/Pawn.h"

UGA_ThrowKunai::UGA_ThrowKunai()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ThrowKunai::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	AActor* Avatar = GetAvatarActorFromActorInfo();
	UWorld* World = Avatar != nullptr ? Avatar->GetWorld() : nullptr;
	if (Avatar == nullptr || World == nullptr || KunaiClass == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	const FVector Forward = Avatar->GetActorForwardVector();
	const FVector SpawnLocation = Avatar->GetActorLocation() + Forward * SpawnForwardOffset + FVector(0.f, 0.f, SpawnHeight);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = Avatar;
	SpawnParams.Instigator = Cast<APawn>(Avatar);
	
	AStealthKunai* Kunai = World->SpawnActor<AStealthKunai>(KunaiClass, SpawnLocation, Forward.Rotation(), SpawnParams);
	if (Kunai != nullptr)
	{
		Kunai->Launch(Forward);
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
