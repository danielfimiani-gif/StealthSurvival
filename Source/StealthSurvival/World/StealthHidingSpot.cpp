#include "StealthHidingSpot.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/StealthSurvivalCharacter.h"

AStealthHidingSpot::AStealthHidingSpot()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	FurnitureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FurnitureMesh"));
	FurnitureMesh->SetupAttachment(Root);
	
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(Root);
	InteractionVolume->SetBoxExtent(FVector(120.0f, 120.0f, 100.0f));
	InteractionVolume->SetCollisionProfileName(TEXT("Trigger"));
	InteractionVolume->SetGenerateOverlapEvents(true);
	
	HideSlot = CreateDefaultSubobject<USceneComponent>(TEXT("HideSlot"));
	HideSlot->SetupAttachment(Root);
	
	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AStealthHidingSpot::OnVolumeBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AStealthHidingSpot::OnVolumeEndOverlap);
	
	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComp"));
}

void AStealthHidingSpot::Interact(AActor* Interactor)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(Interactor))
	{
		Player->EnterHidingSpot(this, HideSlot->GetComponentTransform());
	}
}

void AStealthHidingSpot::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->SetInteractableInRange(this); 
	}
}

void AStealthHidingSpot::OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->ClearInteractableInRange(this); 
	}
}

void AStealthHidingSpot::OnBeginFocus()
{
	if (InteractableComp != nullptr)
	{
		InteractableComp->ShowFocus();
	}
}

void AStealthHidingSpot::OnEndFocus()
{
	if (InteractableComp != nullptr)
	{
		InteractableComp->HideFocus();
	}
}