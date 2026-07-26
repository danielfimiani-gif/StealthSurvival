#include "StealthEndScreenWidget.h"

#include "Kismet/GameplayStatics.h"
#include "GameMode/StealthSurvivalGameState.h"
#include "Subsystems/StealthProgressSubsystem.h"
#include "Engine/GameInstance.h"

bool UStealthEndScreenWidget::IsVictory() const
{
	const AStealthSurvivalGameState* GS = GetStealthGameState();
	return GS != nullptr && GS->GetMatchState() == EStealthMatchState::Won;
}

void UStealthEndScreenWidget::RestartLevel()
{
	UGameplayStatics::SetGamePaused(this, false);
	const FName CurrentLevel(*UGameplayStatics::GetCurrentLevelName(this));
	UGameplayStatics::OpenLevel(this, CurrentLevel);
}

void UStealthEndScreenWidget::OpenMainMenu()
{
	UGameplayStatics::SetGamePaused(this, false);
	UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}

AStealthSurvivalGameState* UStealthEndScreenWidget::GetStealthGameState() const
{
	UWorld* World = GetWorld();
	return World != nullptr ? World->GetGameState<AStealthSurvivalGameState>() : nullptr;
}

FText UStealthEndScreenWidget::GetResultTimeText() const
{
	const AStealthSurvivalGameState* GS = GetStealthGameState();
	const float T = GS != nullptr ? GS->GetMatchTime() : 0.f;
	return UStealthProgressSubsystem::FormatTime(T);
}

FText UStealthEndScreenWidget::GetBestTimeText() const
{
	const UStealthProgressSubsystem* Progress = GetProgressSubsystem();
	if (Progress == nullptr || !Progress->HasBestTime())
	{
		return FText::FromString(TEXT("--:--"));
	}
	
	return UStealthProgressSubsystem::FormatTime(Progress->GetBestEscapeTime());
}

UStealthProgressSubsystem* UStealthEndScreenWidget::GetProgressSubsystem() const
{
	UGameInstance* GI = GetGameInstance();
	return GI != nullptr ? GI->GetSubsystem<UStealthProgressSubsystem>() : nullptr;
}
