#include "StealthCameraSwitch.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/StealthSurvivalCharacter.h"
#include "AI/SecurityCamera/StealthSecurityCamera.h"

AStealthCameraSwitch::AStealthCameraSwitch()
{
      PrimaryActorTick.bCanEverTick = false;

      SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
      RootComponent = SwitchMesh;
      SwitchMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

      InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
      InteractionVolume->SetupAttachment(SwitchMesh);
      InteractionVolume->SetBoxExtent(FVector(100.f, 100.f, 100.f));
      InteractionVolume->SetCollisionProfileName(TEXT("Trigger"));
      InteractionVolume->SetGenerateOverlapEvents(true);
      InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AStealthCameraSwitch::OnVolumeBeginOverlap);
      InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AStealthCameraSwitch::OnVolumeEndOverlap);

      InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComp"));
}

void AStealthCameraSwitch::Interact(AActor* Interactor)
{
      if (Cast<AStealthSurvivalCharacter>(Interactor) == nullptr)
      {
              return;
      }

      bCamerasEnabled = !bCamerasEnabled;

      for (AStealthSecurityCamera* Camera : LinkedCameras)
      {
              if (Camera != nullptr)
              {
                      Camera->SetCameraActive(bCamerasEnabled);
              }
      }
}

void AStealthCameraSwitch::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
      if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
      {
              Player->SetInteractableInRange(this);
      }
}

void AStealthCameraSwitch::OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->ClearInteractableInRange(this);
	}
}

void AStealthCameraSwitch::OnBeginFocus()
{
      if (InteractableComp != nullptr)
      {
              InteractableComp->ShowFocus();
      }
}

void AStealthCameraSwitch::OnEndFocus()
{
      if (InteractableComp != nullptr)
      {
              InteractableComp->HideFocus();
      }
}