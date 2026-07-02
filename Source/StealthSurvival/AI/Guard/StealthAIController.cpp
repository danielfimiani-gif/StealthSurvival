#include "StealthAIController.h"

#include "Player/StealthSurvivalCharacter.h"
#include "GameMode/StealthSurvivalGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "StealthAlertSubsystem.h"
#include "StealthGuardCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "GenericTeamAgentInterface.h"

AStealthAIController::AStealthAIController(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.f;
	SightConfig->LoseSightRadius = 1800.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1500.f;
	HearingConfig->SetMaxAge(3.f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
	
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->ConfigureSense(*HearingConfig);
	PerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AStealthAIController::OnTargetPerceptionUpdated);
	PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AStealthAIController::OnTargetForgotten);
}

void AStealthAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	SetGenericTeamId(FGenericTeamId(1));
	
	AStealthGuardCharacter* Guard = Cast<AStealthGuardCharacter>(InPawn);
	if (Guard == nullptr) return;

	if (SightConfig != nullptr && PerceptionComponent != nullptr)
	{
		SightConfig->SightRadius = Guard->GetSightRange();
		SightConfig->LoseSightRadius = Guard->GetSightRange() + 300.f;
		SightConfig->PeripheralVisionAngleDegrees = Guard->GetSightHalfAngle();
		PerceptionComponent->ConfigureSense(*SightConfig);
		PerceptionComponent->RequestStimuliListenerUpdate();
	}

	UBehaviorTree* BT = Guard->GetBehaviorTree();
	if (BT == nullptr) return;
	
	if (BT->GetBlackboardAsset())
	{
		UBlackboardComponent* BBComp = nullptr;
		UseBlackboard(BT->GetBlackboardAsset(), BBComp);
	}
	
	if (UWorld* World = GetWorld())
	{
		if (UStealthAlertSubsystem* Alert = World->GetSubsystem<UStealthAlertSubsystem>())
		{
			Alert->OnAlertRaised.AddDynamic(this, &AStealthAIController::OnAlertReceived);
		}
	}
	
	RunBehaviorTree(BT);
}

void AStealthAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SetWatchingPlayer(false);
	
	if (UWorld* World = GetWorld())
	{
		if (UStealthAlertSubsystem* Alert = World->GetSubsystem<UStealthAlertSubsystem>())
		{
			Alert->OnAlertRaised.RemoveDynamic(this, &AStealthAIController::OnAlertReceived);
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void AStealthAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor == nullptr || Blackboard == nullptr)
	{
		return;
	}
	
	static const FName TargetActorKey = TEXT("TargetActor");
	static const FName LastKnownLocationKey = TEXT("LastKnownLocation");
	static const FName HasLineOfSightKey = TEXT("HasLineOfSight");
	
	AActor* CurrentTarget = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey));
	
	const bool bIsSight = (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>()); 
	
	if (!bIsSight && Stimulus.WasSuccessfullySensed() && CurrentTarget != nullptr && CurrentTarget != Actor)
	{
		return;
	}
	
	if (Stimulus.WasSuccessfullySensed())
	{
		Blackboard->SetValueAsObject(TargetActorKey, Actor);
		Blackboard->SetValueAsVector(LastKnownLocationKey, Stimulus.StimulusLocation);
		
		if (bIsSight)
		{
			Blackboard->SetValueAsBool(HasLineOfSightKey, true);
		}
	}
	else
	{
		if (bIsSight)
		{
			Blackboard->SetValueAsBool(HasLineOfSightKey, false);
			Blackboard->SetValueAsVector(LastKnownLocationKey, Stimulus.StimulusLocation);
		}
		
		if (Stimulus.IsExpired() && CurrentTarget  == Actor)
		{
			Blackboard->ClearValue(TargetActorKey);
			Blackboard->SetValueAsBool(HasLineOfSightKey, false);
		}
	}
	
	if (bIsSight && Actor->IsA<AStealthSurvivalCharacter>())
	{
		SetWatchingPlayer(Stimulus.WasSuccessfullySensed());
	}
}

void AStealthAIController::OnAlertReceived(FVector AlertLocation, AActor* AlertInstigator)
{
	if (Blackboard == nullptr)
	{
		return;
	}
	
	static const FName TargetActorKey = TEXT("TargetActor");
	static const FName InvestigateLocationKey = TEXT("InvestigateLocation");
	
	if (Blackboard->GetValueAsObject(TargetActorKey) != nullptr)
	{
		return;
	}
	
	Blackboard->SetValueAsVector(InvestigateLocationKey, AlertLocation);
}

ETeamAttitude::Type AStealthAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
	if (!TeamAgent)
	{
		return ETeamAttitude::Neutral;
	}
	
	if (TeamAgent->GetGenericTeamId() == GetGenericTeamId())
	{
		return ETeamAttitude::Friendly;
	}

	return ETeamAttitude::Hostile;
}

void AStealthAIController::OnTargetForgotten(AActor* Actor)
{
	if (Actor == nullptr || Blackboard == nullptr)
	{
		return;
	}
	
	static const FName TargetActorKey = TEXT("TargetActor");
	static const FName HasLineOfSightKey = TEXT("HasLineOfSight");
	
	if (Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey)) == Actor)
	{
		Blackboard->ClearValue(TargetActorKey);
		Blackboard->SetValueAsBool(HasLineOfSightKey, false);
	}
}

void AStealthAIController::SetWatchingPlayer(bool bNewWatching)
{
	if (bIsWatchingPlayer == bNewWatching)
	{
		return;
	}
	
	bIsWatchingPlayer = bNewWatching;
	
	if (AStealthSurvivalGameMode* GM = Cast<AStealthSurvivalGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (bNewWatching)
		{
			GM->AddWatcher();
		}
		else
		{
			GM->RemoveWatcher();
		}
	}
}
