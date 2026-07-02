#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

class UWidgetComponent;
class UUserWidget;

UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractableComponent();
	
	void ShowFocus();
	void HideFocus();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly,  Category="Interactable")
	int32 StencilValue = 1;
	
	UPROPERTY(EditDefaultsOnly,  Category="Interactable")
	TSubclassOf<UUserWidget> PromptWidgetClass;
	
	UPROPERTY(EditDefaultsOnly,  Category="Interactable")
	FVector PromptOffset = FVector(0.f, 0.f, 100.f);
	
	UPROPERTY()
	TObjectPtr<UWidgetComponent> PromptWidget;
	
private:
	void SetMeshCustomDepth(bool enabled);
};
