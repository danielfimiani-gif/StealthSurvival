#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StealthThrowable.generated.h"

class UStaticMeshComponent;

UCLASS(Abstract)
class AStealthThrowable : public AActor
{
	GENERATED_BODY()

public:
	AStealthThrowable();
	void Launch(const FVector& InitialVelocity);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Stealth|Noise", meta = (ClampMin="0"))
	float NoiseLoudness = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Stealth|Noise", meta = (ClampMin="0"))
	float NoiseRange = 1800.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Stealth", meta = (ClampMin="0"))
	float LifeSpanAfterHit = 5.f;
	
	bool bHasReportedNoise = false;
	
	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
