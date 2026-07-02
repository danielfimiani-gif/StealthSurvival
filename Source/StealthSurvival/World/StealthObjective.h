#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "Interaction/InteractableComponent.h"
#include "StealthObjective.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class AStealthObjective : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AStealthObjective();
	
	virtual  void Interact(AActor* Interactor) override;
	virtual void OnBeginFocus() override;
	virtual void OnEndFocus() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective")
	TObjectPtr<UStaticMeshComponent> ObjectiveMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective")
	TObjectPtr<UBoxComponent> InteractionVolume;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective")
	TObjectPtr<UInteractableComponent> InteractableComp;
	
	UFUNCTION()
	void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult&  SweepResult);
	
	UFUNCTION()
	void OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
