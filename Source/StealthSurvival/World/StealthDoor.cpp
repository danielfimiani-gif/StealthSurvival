#include "StealthDoor.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/StealthSurvivalCharacter.h"
#include "GameMode/StealthSurvivalGameState.h"
#include "SmartObjectComponent.h"

AStealthDoor::AStealthDoor()
{
      PrimaryActorTick.bCanEverTick = false;

      DoorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorRoot"));
      RootComponent = DoorRoot;

      DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
      DoorFrameMesh->SetupAttachment(DoorRoot);
      DoorFrameMesh->SetCollisionProfileName(TEXT("BlockAll"));
      
      DoorLeafMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeafMesh"));
      DoorLeafMesh->SetupAttachment(DoorRoot);
      DoorLeafMesh->SetCollisionProfileName(TEXT("BlockAll"));

      InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
      InteractionVolume->SetupAttachment(DoorRoot);
      InteractionVolume->SetBoxExtent(FVector(150.f, 150.f, 100.f));
      InteractionVolume->SetCollisionProfileName(TEXT("Trigger"));
      InteractionVolume->SetGenerateOverlapEvents(true);
      InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AStealthDoor::OnVolumeBeginOverlap);
      InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AStealthDoor::OnVolumeEndOverlap);

      InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComp"));
      
      SmartObjectComp = CreateDefaultSubobject<USmartObjectComponent>(TEXT("SmartObjectComp"));
      SmartObjectComp->SetupAttachment(DoorRoot);
}

void AStealthDoor::Interact(AActor* Interactor)
{
      AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(Interactor);
      if (Player == nullptr)
      {
            return;
      }
      
      if (bIsOpen)
      {
            CloseDoor();
            return;
      }
      
      const bool bNeedsKey = !RequiredKeyId.IsNone();
      bool bHasKey = false;
      if (bNeedsKey)
      {
            const AStealthSurvivalGameState* GS = GetWorld() != nullptr
                  ? GetWorld()->GetGameState<AStealthSurvivalGameState>()
                  : nullptr;
            bHasKey = GS != nullptr && GS->HasKey(RequiredKeyId);
      }
      
      if (!bNeedsKey || bHasKey)
      {
            OpenDoor();
      }
      else
      {
            OnDoorLocked();
      }
}

void AStealthDoor::OpenDoor()
{
      bIsOpen = true;
      if (DoorLeafMesh != nullptr)
      {
            DoorLeafMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
      }
      
      OnDoorOpened();
}

void AStealthDoor::CloseDoor()
{
      bIsOpen = false;
      if (DoorLeafMesh != nullptr)
      {
            DoorLeafMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
      }
      OnDoorClosed();
}

void AStealthDoor::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
      if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
      {
            Player->SetInteractableInRange(this);
      }
}

void AStealthDoor::OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
      if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
      {
            Player->ClearInteractableInRange(this);
      }
}

void AStealthDoor::OnBeginFocus()
{
      if (InteractableComp != nullptr)
      {
            InteractableComp->ShowFocus();
      }
}

void AStealthDoor::OnEndFocus()
{
      if (InteractableComp != nullptr)
      {
            InteractableComp->HideFocus();
      }
}

void AStealthDoor::OpenForAI()
{
      if (!bIsOpen)
      {
            OpenDoor();
      }
}
