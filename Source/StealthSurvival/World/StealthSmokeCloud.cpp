#include "StealthSmokeCloud.h"

#include "Components/SphereComponent.h"
#include "Player/StealthSurvivalCharacter.h"

AStealthSmokeCloud::AStealthSmokeCloud()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	SmokeVolume = CreateDefaultSubobject<USphereComponent>(TEXT("SmokeVolume"));
	SmokeVolume->SetupAttachment(SceneRoot);
	SmokeVolume->SetSphereRadius(400.f);
	SmokeVolume->SetCollisionProfileName(TEXT("Trigger"));
	SmokeVolume->SetGenerateOverlapEvents(true);
}

void AStealthSmokeCloud::BeginPlay()
{
	Super::BeginPlay();
	
	SmokeVolume->OnComponentBeginOverlap.AddDynamic(this, &AStealthSmokeCloud::OnVolumeBeginOverlap);
	SmokeVolume->OnComponentEndOverlap.AddDynamic(this, &AStealthSmokeCloud::OnVolumeEndOverlap);
	
	SetLifeSpan(Duration);
}

void AStealthSmokeCloud::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<AActor*> Inside;
	SmokeVolume->GetOverlappingActors(Inside, AStealthSurvivalCharacter::StaticClass());
	for (AActor* Actor : Inside)
	{
		if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(Actor))
		{
			Player->RemoveSmokeSource();
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void AStealthSmokeCloud::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->AddSmokeSource();
	}
}

void AStealthSmokeCloud::OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->RemoveSmokeSource();
	}
}


