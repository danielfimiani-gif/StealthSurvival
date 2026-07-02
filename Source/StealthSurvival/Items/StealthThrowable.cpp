#include "StealthThrowable.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include  "Perception/AISense_Hearing.h"

AStealthThrowable::AStealthThrowable()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->OnComponentHit.AddDynamic(this, &AStealthThrowable::OnMeshHit);
}

void AStealthThrowable::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bHasReportedNoise)
	{
		return;
	}
	bHasReportedNoise = true;
	
	UAISense_Hearing::ReportNoiseEvent(
		GetWorld(),
		GetActorLocation(),
		NoiseLoudness,
		GetInstigator(),
		NoiseRange
	);
	
	SetLifeSpan(LifeSpanAfterHit);
}

void AStealthThrowable::Launch(const FVector& InitialVelocity)
{
	if (MeshComponent == nullptr)
	{
		return;
	}
	MeshComponent->SetPhysicsLinearVelocity(InitialVelocity);
}
