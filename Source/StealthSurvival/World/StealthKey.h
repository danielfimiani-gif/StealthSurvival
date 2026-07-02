#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "Interaction/InteractableComponent.h"
#include "StealthKey.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class AStealthKey : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AStealthKey();

	virtual void Interact(AActor* Interactor) override;
	virtual void OnBeginFocus() override;
	virtual void OnEndFocus() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Key")
	FName KeyId = TEXT("MainKey");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Key")
	TObjectPtr<UStaticMeshComponent> KeyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Key")
	TObjectPtr<UBoxComponent> InteractionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Key")
	TObjectPtr<UInteractableComponent> InteractableComp;

	UFUNCTION()
	void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};