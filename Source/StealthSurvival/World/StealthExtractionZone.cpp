#include "StealthExtractionZone.h"

#include "Components/BoxComponent.h"
#include "Player/StealthSurvivalCharacter.h"
#include "GameMode/StealthSurvivalGameMode.h"
#include "Kismet/GameplayStatics.h"

AStealthExtractionZone::AStealthExtractionZone()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ExtractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ExtractionVolume"));
	RootComponent = ExtractionVolume;
	ExtractionVolume->SetBoxExtent(FVector(200.f, 200.f, 100.f));
	ExtractionVolume->SetCollisionProfileName(TEXT("Trigger"));
	ExtractionVolume->SetGenerateOverlapEvents(true);
	ExtractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AStealthExtractionZone::OnVolumeBeginOverlap);
}

void AStealthExtractionZone::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AStealthSurvivalCharacter>())
	{
		if (AStealthSurvivalGameMode* GM = Cast<AStealthSurvivalGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->NotifyPlayerReachedExit();
		}
	}
}
