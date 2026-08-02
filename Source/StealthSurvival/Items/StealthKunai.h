#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StealthKunai.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS(abstract)
class AStealthKunai : public AActor
{
	GENERATED_BODY()
	
public:
	AStealthKunai();
	
	void Launch(const FVector& Direction);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> CollisionComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Kunai", meta=(ClampMin="0"))
	float Speed = 3000.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Kunai", meta=(ClampMin="0"))
	float StunDuration = 4.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Kunai", meta=(ClampMin="0"))
	float MaxLifeSpanSeconds = 3.f;
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitInfo);
};
