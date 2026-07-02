#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StealthExtractionZone.generated.h"

class UBoxComponent;

UCLASS()
class AStealthExtractionZone : public AActor
{
	GENERATED_BODY()
public:
	AStealthExtractionZone();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Extraction")
	TObjectPtr<UBoxComponent> ExtractionVolume;
	
	UFUNCTION()
	void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult&  SweepResult);
};
