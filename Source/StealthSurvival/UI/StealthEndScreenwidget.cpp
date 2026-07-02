#include "StealthEndScreenWidget.h"

#include "Kismet/GameplayStatics.h"
#include "GameMode/StealthSurvivalGameState.h"

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