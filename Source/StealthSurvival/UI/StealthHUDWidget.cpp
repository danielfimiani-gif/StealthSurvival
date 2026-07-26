#include "StealthHUDWidget.h"

#include "GameMode/StealthSurvivalGameState.h"
#include "Player/StealthSurvivalCharacter.h"
#include "AbilitySystemComponent.h"
#include "Subsystems/StealthItemRegistrySubsystem.h"
#include "Engine/GameInstance.h"

float UStealthHUDWidget::GetDetectionLevel() const
{
	const AStealthSurvivalGameState* GS = GetStealthGameState();
	return GS != nullptr ? GS->GetDetectionLevel() : 0.0f;
}

FText UStealthHUDWidget::GetObjectiveText() const
{
	const AStealthSurvivalGameState* GS = GetStealthGameState();

	FName ActiveId;
	FText Fallback;
	if (GS == nullptr || !GS->HasKey(AccessCardId))
	{
		ActiveId = AccessCardId;
		Fallback = CardObjectiveText;
	}
	else if (!GS->HasObjective())
	{
		ActiveId = ObjectiveItemId;
		Fallback = StealObjectiveText;
	}
	else if (!GS->HasKey(EscapeCodeId))
	{
		ActiveId = EscapeCodeId;
		Fallback = FindCodeObjectiveText;
	}
	else
	{
		ActiveId = EscapeStepId;
		Fallback = EscapeObjectiveText;
	}

	if (const UStealthItemRegistrySubsystem* Registry = GetItemRegistry())
	{
		const FText Text = Registry->GetObjectiveText(ActiveId);
		if (!Text.IsEmpty())
		{
				return Text;
		}
	}
	return Fallback;
}

bool UStealthHUDWidget::IsPlayerConcealed() const
{
	AStealthSurvivalCharacter* Player = GetStealthPlayer();
	if (Player == nullptr)
	{
		return false;
	}
	return Player->IsInCover() || Player->IsHidden() || Player->IsInvisible();
}

bool UStealthHUDWidget::IsPlayerInvisible() const
{
	AStealthSurvivalCharacter* Player = GetStealthPlayer();
	return Player != nullptr && Player->IsInvisible();
}

float UStealthHUDWidget::GetInvisibilityCooldownPercent() const
{
	AStealthSurvivalCharacter* Player = GetStealthPlayer();
	if (Player == nullptr)
	{
		return 0.f;
	}

	UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return 0.f;
	}

	FGameplayTagContainer CooldownTags;
	CooldownTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Invisibility")));

	const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
	const TArray<TPair<float, float>> TimeRemainingAndDuration = ASC->GetActiveEffectsTimeRemainingAndDuration(Query);

	if (TimeRemainingAndDuration.Num() == 0)
	{
		return 0.f;
	}

	const float Remaining = TimeRemainingAndDuration[0].Key;
	const float Duration = TimeRemainingAndDuration[0].Value;
	return Duration > 0.f ? (Remaining / Duration) : 0.f;
}

AStealthSurvivalGameState* UStealthHUDWidget::GetStealthGameState() const
{
	UWorld* World = GetWorld();
	return World != nullptr ? World->GetGameState<AStealthSurvivalGameState>() : nullptr;
}

AStealthSurvivalCharacter* UStealthHUDWidget::GetStealthPlayer() const
{
	return Cast<AStealthSurvivalCharacter>(GetOwningPlayerPawn());
}

FText UStealthHUDWidget::GetMatchTimeText() const
{
	const AStealthSurvivalGameState* GS = GetStealthGameState();
	const float T = GS != nullptr ? GS->GetMatchTime() : 0.f;
	
	const int32 Minutes = FMath::FloorToInt(T / 60.f);
	const int32 Seconds = FMath::FloorToInt(T) % 60;
	return FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds));
}

const UStealthItemRegistrySubsystem* UStealthHUDWidget::GetItemRegistry() const
{
	const UGameInstance* GI = GetGameInstance();
	return GI != nullptr ? GI->GetSubsystem<UStealthItemRegistrySubsystem>() : nullptr;
}
