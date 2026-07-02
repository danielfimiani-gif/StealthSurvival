#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class IInteractable
{
	GENERATED_BODY()
public:
	virtual void Interact(AActor* Interactor) = 0;
	
	virtual void OnBeginFocus () {}
	virtual void OnEndFocus () {}
};
