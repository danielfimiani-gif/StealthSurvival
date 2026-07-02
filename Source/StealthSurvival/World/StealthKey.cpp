#include "StealthKey.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/StealthSurvivalCharacter.h"
#include "GameMode/StealthSurvivalGameMode.h"
#include "Kismet/GameplayStatics.h"

AStealthKey::AStealthKey()
{
	PrimaryActorTick.bCanEverTick = false;

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	RootComponent = KeyMesh;
	KeyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(KeyMesh);
	InteractionVolume->SetBoxExtent(FVector(120.f, 120.f, 100.f));
	InteractionVolume->SetCollisionProfileName(TEXT("Trigger"));
	InteractionVolume->SetGenerateOverlapEvents(true);
	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AStealthKey::OnVolumeBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AStealthKey::OnVolumeEndOverlap);

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComp"));
}

void AStealthKey::Interact(AActor* Interactor)
{
	AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(Interactor);
	if (Player == nullptr)
	{
		  return;
	}

	if (AStealthSurvivalGameMode* GM =Cast<AStealthSurvivalGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		  GM->NotifyKeyCollected(KeyId);
	}

	Player->ClearInteractableInRange(this);
	Destroy();
}

void AStealthKey::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->SetInteractableInRange(this);
	}
}

void AStealthKey::OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->ClearInteractableInRange(this);
	}
}

void AStealthKey::OnBeginFocus()
{
	if (InteractableComp != nullptr)
	{
		InteractableComp->ShowFocus();
	}
}

void AStealthKey::OnEndFocus()
{
	if (InteractableComp != nullptr)
	{
		InteractableComp->HideFocus();
	}
}
