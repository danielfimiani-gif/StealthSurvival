#include "StealthHUDWidget.h"

#include "GameMode/StealthSurvivalGameState.h"
#include "Player/StealthSurvivalCharacter.h"
#include "AbilitySystemComponent.h"

float UStealthHUDWidget::GetDetectionLevel() const
{
	const AStealthSurvivalGameState* GS = GetStealthGameState();
	return GS != nullptr ? GS->GetDetectionLevel() : 0.0f;
}

FText UStealthHUDWidget::GetObjectiveText() const
{
	const AStealthSurvivalGameState* GS = GetStealthGameState();
	if (GS == nullptr)
	{
		return CardObjectiveText;
	}

	if (!GS->HasKey(AccessCardId))
	{
		return CardObjectiveText;
	}

	if (!GS->HasObjective())
	{
		return StealObjectiveText;
	}

	if (!GS->HasKey(EscapeCodeId))
	{
		return FindCodeObjectiveText;
	}

	return EscapeObjectiveText;
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
