#include "StealthMainMenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UStealthMainMenuWidget::PlayGame()
{
	UGameplayStatics::OpenLevel(this, GameplayLevelName);
}

void UStealthMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
