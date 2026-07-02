#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "Interaction/InteractableComponent.h"
#include "StealthDoor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USmartObjectComponent;

UCLASS()
class AStealthDoor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AStealthDoor();

	virtual void Interact(AActor* Interactor) override;
	virtual void OnBeginFocus() override;
	virtual void OnEndFocus() override;
	void OpenForAI();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	FName RequiredKeyId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	TObjectPtr<USceneComponent> DoorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	TObjectPtr<UStaticMeshComponent> DoorFrameMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	TObjectPtr<UStaticMeshComponent> DoorLeafMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	TObjectPtr<UBoxComponent> InteractionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	TObjectPtr<UInteractableComponent> InteractableComp;

	UPROPERTY(BlueprintReadOnly, Category="Door")
	bool bIsOpen = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	TObjectPtr<USmartObjectComponent> SmartObjectComp;

	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnDoorOpened();

	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnDoorClosed();

	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnDoorLocked();
	
	void OpenDoor();
	void CloseDoor();
	
	UFUNCTION()
	void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};