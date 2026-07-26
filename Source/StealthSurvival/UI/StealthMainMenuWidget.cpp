#include "StealthMainMenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystems/StealthProgressSubsystem.h"
#include "Engine/GameInstance.h"

void UStealthMainMenuWidget::PlayGame()
{
	UGameplayStatics::OpenLevel(this, GameplayLevelName);
}

void UStealthMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UStealthMainMenuWidget::SetDifficulty(EStealthDifficulty NewDifficulty)
{
	if (UStealthProgressSubsystem* Progress = GetProgressSubsystem())
	{
		Progress->SetDifficulty(NewDifficulty);
	}
}

EStealthDifficulty UStealthMainMenuWidget::GetDifficulty() const
{
	const UStealthProgressSubsystem* Progress = GetProgressSubsystem();
	return Progress != nullptr ? Progress->GetDifficulty() : EStealthDifficulty::Normal;
}

FText UStealthMainMenuWidget::GetBestTimeText() const
{
	const UStealthProgressSubsystem* Progress = GetProgressSubsystem();
	if (Progress == nullptr || !Progress->HasBestTime())
	{
		return FText::FromString(TEXT("--:--"));
	}
	
	return UStealthProgressSubsystem::FormatTime(Progress->GetBestEscapeTime());
}

UStealthProgressSubsystem* UStealthMainMenuWidget::GetProgressSubsystem() const
{
	UGameInstance* GI = GetGameInstance();
	return GI != nullptr ? GI->GetSubsystem<UStealthProgressSubsystem>() : nullptr;
}
