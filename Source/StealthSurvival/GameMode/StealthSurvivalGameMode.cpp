#include "StealthSurvivalGameMode.h"
#include "StealthSurvivalGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/StealthSurvivalPlayerController.h"
#include "Subsystems/StealthProgressSubsystem.h"
#include "Engine/GameInstance.h"

AStealthSurvivalGameMode::AStealthSurvivalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	GameStateClass = AStealthSurvivalGameState::StaticClass();
}

void AStealthSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (const UStealthProgressSubsystem* Progress = GetProgressSubsystem())
	{
		DetectionRiseRate *= Progress->GetDetectionMultiplier();
	}
}

AStealthSurvivalGameState* AStealthSurvivalGameMode::GetStealthSurvivalGameState() const
{
	return GetGameState<AStealthSurvivalGameState>();
}

void AStealthSurvivalGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AStealthSurvivalGameState* GS = GetStealthSurvivalGameState();
	if (GS == nullptr)
	{
		return;
	}
	
	if (GS->GetMatchState() != EStealthMatchState::Playing)
	{
		return;
	}
	
	GS->AddMatchTime(DeltaSeconds);
	
	const float Delta = (ActiveWatchers > 0)
		? DetectionRiseRate * DeltaSeconds
		: -DetectionFallRate * DeltaSeconds;
	
	const float NewLevel = GS->GetDetectionLevel() + Delta;
	GS->SetDetectionLevel(NewLevel);
	
	if (NewLevel >= 1.f)
	{
		TriggerLose();
	}
}

void AStealthSurvivalGameMode::AddWatcher()
{
	++ActiveWatchers;
}

void AStealthSurvivalGameMode::RemoveWatcher()
{
	ActiveWatchers = FMath::Max(0, ActiveWatchers - 1);
}

void AStealthSurvivalGameMode::NotifyObjectiveStolen()
{
	if (AStealthSurvivalGameState* GS = GetStealthSurvivalGameState())
	{
		GS->SetHasObjective(true);
	}
}

void AStealthSurvivalGameMode::NotifyPlayerReachedExit()
{
	AStealthSurvivalGameState* GS = GetStealthSurvivalGameState();
	if (GS == nullptr)
	{
		return;
	}
	
	if (GS->HasObjective())
	{
		TriggerWin();
	}
}

void AStealthSurvivalGameMode::TriggerWin()
{
	AStealthSurvivalGameState* GS = GetStealthSurvivalGameState();
	if (GS == nullptr || GS->GetMatchState() != EStealthMatchState::Playing)
	{
		return;
	}
	
	GS->SetMatchState(EStealthMatchState::Won);
	
	if (UStealthProgressSubsystem* Progress = GetProgressSubsystem())
	{
		Progress->RecordMatchResult(true, GS->GetMatchTime());
	}
	
	if (AStealthSurvivalPlayerController* PC = Cast<AStealthSurvivalPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->ShowEndScreen();
	}
}

void AStealthSurvivalGameMode::TriggerLose()
{
	AStealthSurvivalGameState* GS = GetStealthSurvivalGameState();
	if (GS == nullptr || GS->GetMatchState() != EStealthMatchState::Playing)
	{
		return;
	}
	
	GS->SetMatchState(EStealthMatchState::Lost);
	GS->SetDetectionLevel(1.f);
	
	if (UStealthProgressSubsystem* Progress = GetProgressSubsystem())
	{
		Progress->RecordMatchResult(false, -1.f);
	}
	
	if (AStealthSurvivalPlayerController* PC = Cast<AStealthSurvivalPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->ShowEndScreen();
	}
}

void AStealthSurvivalGameMode::NotifyKeyCollected(FName KeyId)
{
	if (AStealthSurvivalGameState* GS = GetStealthSurvivalGameState())
	{
		GS->AddKey(KeyId);
	}
}

class UStealthProgressSubsystem* AStealthSurvivalGameMode::GetProgressSubsystem() const
{
	UGameInstance* GI = GetGameInstance();
	return GI != nullptr ? GI->GetSubsystem<UStealthProgressSubsystem>() : nullptr;
}
