#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "Interaction/InteractableComponent.h"
#include "StealthCameraSwitch.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AStealthSecurityCamera;

UCLASS()
class AStealthCameraSwitch : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AStealthCameraSwitch();

	virtual void Interact(AActor* Interactor) override;
	virtual void OnBeginFocus() override;
	virtual void OnEndFocus() override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Switch")
	TArray<TObjectPtr<AStealthSecurityCamera>> LinkedCameras;

	UPROPERTY(BlueprintReadOnly, Category="Switch")
	bool bCamerasEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Switch")
	TObjectPtr<UStaticMeshComponent> SwitchMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Switch")
	TObjectPtr<UBoxComponent> InteractionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Switch")
	TObjectPtr<UInteractableComponent> InteractableComp;

	UFUNCTION()
	void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnVolumeEndOverlap (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};