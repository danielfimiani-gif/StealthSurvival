#include "StealthSecurityCamera.h"

#include "StealthAlertSubsystem.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/StateTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameMode/StealthSurvivalGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Sight.h"

AStealthSecurityCamera::AStealthSecurityCamera()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(SceneRoot);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CameraPivot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraPivot"));
	CameraPivot->SetupAttachment(SceneRoot);
	
	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(CameraPivot);
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	DetectionCone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DetectionCone"));
	DetectionCone->SetupAttachment(CameraPivot);
	DetectionCone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetectionCone->SetCastShadow(false);
	DetectionCone->SetVectorParameterValueOnMaterials(TEXT("ConeColor"), FVector(UnawareColor.R, UnawareColor.G, UnawareColor.B));
	
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle;
	SightConfig->SetMaxAge(SightMaxAge);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
}


void AStealthSecurityCamera::BeginPlay()
{
	Super::BeginPlay();

	if (CameraPivot != nullptr)
	{
		HomeYaw = CameraPivot->GetRelativeRotation().Yaw;
	}

	if (PerceptionComponent != nullptr)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AStealthSecurityCamera::OnPerceptionUpdated);
	}
}

void AStealthSecurityCamera::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (CameraPivot != nullptr)
	{
		OutLocation = CameraPivot->GetComponentLocation();
		OutRotation = CameraPivot->GetComponentRotation();
		return;
	}
	
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}

void AStealthSecurityCamera::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor == nullptr)
	{
		return;
	}
	
	if (!Actor->ActorHasTag(TEXT("Player")))
	{
		return;
	}
	
	if (!bIsActive)
	{
		return;
	}
	
	if (Stimulus.WasSuccessfullySensed())
	{
		const bool bWasUnaware = (TargetActor == nullptr);
		TargetActor = Actor;
		LastSeenLocation = Stimulus.StimulusLocation;
		
		if (bWasUnaware)
		{
			if (UWorld* World = GetWorld())
			{
				if (UStealthAlertSubsystem* Alert = World->GetSubsystem<UStealthAlertSubsystem>())
				{
					Alert->RaiseAlert(LastSeenLocation, TargetActor);
				}
			}
		}
	}
	else
	{
		LastSeenLocation = Stimulus.StimulusLocation;
		TargetActor = nullptr;
	}
	
	SetWatchingPlayer(Stimulus.WasSuccessfullySensed());
	
	if (DetectionCone != nullptr)
	{
		const FLinearColor C = (TargetActor != nullptr) ? AlertedColor : UnawareColor;
		DetectionCone->SetVectorParameterValueOnMaterials(TEXT("ConeColor"), FVector(C.R, C.G, C.B));
	}
}

void AStealthSecurityCamera::SetWatchingPlayer(bool bNewWatching)
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

void AStealthSecurityCamera::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SetWatchingPlayer(false);
	Super::EndPlay(EndPlayReason);
}

void AStealthSecurityCamera::SetCameraActive(bool bActive)
{
	if (bIsActive == bActive)
	{
		return;
	}
	
	bIsActive = bActive;
	
	if (PerceptionComponent != nullptr)
	{
		PerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), bActive);
	}
	
	if (DetectionCone != nullptr)
	{
		DetectionCone->SetVisibility(bActive);
	}
	
	if (StateTreeComponent != nullptr)
	{
		if (bActive)
		{
			StateTreeComponent->StartLogic();
		}
		else
		{
			StateTreeComponent->StopLogic(TEXT("Camera deactivated"));
		}
	}
	
	if (!bActive)
	{
		TargetActor = nullptr;
		SetWatchingPlayer(false);
	}
	
	OnPowerStateChanged(bActive);
}
