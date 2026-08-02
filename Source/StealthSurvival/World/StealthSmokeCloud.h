#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StealthSmokeCloud.generated.h"

class USphereComponent;

UCLASS(abstract)
class AStealthSmokeCloud : public AActor
{
	GENERATED_BODY()
	
public:
	AStealthSmokeCloud();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> SmokeVolume;
	
	UPROPERTY(EditDefaultsOnly, Category="Smoke", meta=(ClampMin="0"))
	float Duration = 6.f;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
