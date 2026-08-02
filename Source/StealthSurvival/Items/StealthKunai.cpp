#include "StealthKunai.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AI/Guard/StealthGuardCharacter.h"
#include "Components/SphereComponent.h"

AStealthKunai::AStealthKunai()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(FName("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitSphereRadius(10.f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(CollisionComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 0.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement-> bShouldBounce = false;
}

void AStealthKunai::BeginPlay()
{
	Super::BeginPlay();
	
	if (AActor* OwnerActor = GetOwner())
	{
		CollisionComponent->IgnoreActorWhenMoving(OwnerActor, true);
	}
	
	CollisionComponent->OnComponentHit.AddDynamic(this, &AStealthKunai::OnMeshHit);
	SetLifeSpan(MaxLifeSpanSeconds);
}

void AStealthKunai::Launch(const FVector& Direction)
{
	if (ProjectileMovement != nullptr)
	{
		ProjectileMovement->Velocity = Direction.GetSafeNormal() * Speed;
	}
}

void AStealthKunai::OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitInfo)
{
	if (AStealthGuardCharacter* Guard = Cast<AStealthGuardCharacter>(OtherActor))
	{
		Guard->Stun(StunDuration);
	}
	
	Destroy();
}
