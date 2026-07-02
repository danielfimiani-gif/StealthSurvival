#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "Interaction/InteractableComponent.h"
#include "StealthHidingSpot.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class AStealthHidingSpot : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	AStealthHidingSpot();
	
	virtual void Interact(AActor* Interactor) override;
	virtual void OnBeginFocus() override;
	virtual void OnEndFocus() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hiding")
	TObjectPtr<UStaticMeshComponent> FurnitureMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hiding")
	TObjectPtr<UBoxComponent> InteractionVolume;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hiding")
	TObjectPtr<USceneComponent> HideSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hiding")
	TObjectPtr<UInteractableComponent> InteractableComp;
	
	UFUNCTION()
	void OnVolumeBeginOverlap(
		UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	);
	
	UFUNCTION()
	void OnVolumeEndOverlap(
		UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex
	);
};
