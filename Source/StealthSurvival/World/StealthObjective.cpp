#include "StealthObjective.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/StealthSurvivalCharacter.h"
#include "GameMode/StealthSurvivalGameMode.h"
#include "Kismet/GameplayStatics.h"

AStealthObjective::AStealthObjective()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ObjectiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectiveMesh"));
	RootComponent = ObjectiveMesh;
	ObjectiveMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteracionVolume"));
	InteractionVolume->SetupAttachment(ObjectiveMesh);
	InteractionVolume->SetBoxExtent(FVector(120.f, 120.f, 100.f));
	InteractionVolume->SetCollisionProfileName(TEXT("Trigger"));
	InteractionVolume->SetGenerateOverlapEvents(true);
	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AStealthObjective::OnVolumeBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AStealthObjective::OnVolumeEndOverlap);
	
	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComp"));
}

void AStealthObjective::Interact(AActor* Interactor)
{
	AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(Interactor);
	if (Player == nullptr)
	{
		return;
	}
	
	if (AStealthSurvivalGameMode* GM = Cast<AStealthSurvivalGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->NotifyObjectiveStolen();
	}
	
	Player->ClearInteractableInRange(this);
	Destroy();
}

void AStealthObjective::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->SetInteractableInRange(this);
	}
}

void AStealthObjective::OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->ClearInteractableInRange(this);
	}
}

void AStealthObjective::OnBeginFocus()
{
	if (InteractableComp != nullptr)
	{
		InteractableComp->ShowFocus();
	}
}

void AStealthObjective::OnEndFocus()
{
	if (InteractableComp != nullptr)
	{
		InteractableComp->HideFocus();
	}
}