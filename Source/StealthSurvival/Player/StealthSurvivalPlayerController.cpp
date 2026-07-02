// Copyright Epic Games, Inc. All Rights Reserved.


#include "StealthSurvivalPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "StealthSurvival.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "UI/StealthHUDWidget.h"
#include "UI/StealthEndScreenWidget.h"
#include "Kismet/GameplayStatics.h"

void AStealthSurvivalPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	
	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);
		} 
		else 
		{
			UE_LOG(LogStealthSurvival, Error, TEXT("Could not spawn mobile controls widget."));
		}
	}
	
	if (HUDWidgetClass != nullptr)
	{
		HUDWidget = CreateWidget<UStealthHUDWidget>(this, HUDWidgetClass);
		if (HUDWidget != nullptr)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void AStealthSurvivalPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void AStealthSurvivalPlayerController::ShowEndScreen()
{
	if (EndScreenWidget != nullptr || EndScreenWidgetClass == nullptr)
	{
		return;
	}
	
	EndScreenWidget = CreateWidget<UStealthEndScreenWidget>(this, EndScreenWidgetClass);
	if (EndScreenWidget == nullptr)
	{
		return;
	}
	
	EndScreenWidget->AddToViewport(10);
	bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(EndScreenWidget->TakeWidget());
	SetInputMode(InputMode);
	
	UGameplayStatics::SetGamePaused(this, true);
}
