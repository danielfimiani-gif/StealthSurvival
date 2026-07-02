#include "StealthCoverBush.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/StealthSurvivalCharacter.h"

AStealthCoverBush::AStealthCoverBush()
{
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	CoverVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CoverVolume"));
	CoverVolume->SetupAttachment(Root);
	CoverVolume->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	CoverVolume->SetBoxExtent(FVector(50.f,50.f,50.f));
	CoverVolume->SetCollisionProfileName(TEXT("Trigger"));
	CoverVolume->SetGenerateOverlapEvents(true);
	
	BushMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BushMesh"));
	BushMesh->SetupAttachment(Root);
	BushMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CoverVolume->OnComponentBeginOverlap.AddDynamic(this, &AStealthCoverBush::OnVolumeBeginOverlap);
	CoverVolume->OnComponentEndOverlap.AddDynamic(this, &AStealthCoverBush::OnVolumeEndOverlap);
}

void AStealthCoverBush::OnVolumeBeginOverlap(
	UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->AddCoverSource();
	}
}

void AStealthCoverBush::OnVolumeEndOverlap(
	UPrimitiveComponent* OverlapComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AStealthSurvivalCharacter* Player = Cast<AStealthSurvivalCharacter>(OtherActor))
	{
		Player->RemoveCoverSource();
	}
}
