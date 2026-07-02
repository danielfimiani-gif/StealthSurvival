#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StealthCoverBush.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class AStealthCoverBush : public AActor
{
	GENERATED_BODY()
	
public:
	AStealthCoverBush();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cover")
	TObjectPtr<UBoxComponent> CoverVolume;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cover")
	TObjectPtr<UStaticMeshComponent> BushMesh;
	
	UFUNCTION()
	void OnVolumeBeginOverlap(
		UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	);
	
	UFUNCTION()
	void OnVolumeEndOverlap(
		UPrimitiveComponent* OverlapComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	);
};
