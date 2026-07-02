#include "InteractableComponent.h"

#include "Components/WidgetComponent.h"
#include "Components/MeshComponent.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (PromptWidgetClass != nullptr && GetOwner() != nullptr)
	{
		PromptWidget = NewObject<UWidgetComponent>(GetOwner());
		PromptWidget->SetupAttachment(GetOwner()->GetRootComponent());
		PromptWidget->RegisterComponent();
		
		PromptWidget->SetWidgetClass(PromptWidgetClass);
		PromptWidget->SetWidgetSpace(EWidgetSpace::Screen);
		
		PromptWidget->SetDrawAtDesiredSize(true);
		PromptWidget->SetRelativeLocation(PromptOffset);
		PromptWidget->SetVisibility(false);
	}
}

void UInteractableComponent::ShowFocus()
{
	SetMeshCustomDepth(true);
	if (PromptWidget != nullptr)
	{
		PromptWidget->SetVisibility(true);
	}
}

void UInteractableComponent::HideFocus()
{
	SetMeshCustomDepth(false);
	if (PromptWidget != nullptr)
	{
		PromptWidget->SetVisibility(false);
	}
}

void UInteractableComponent::SetMeshCustomDepth(bool bEnabled)
{
	AActor* Owner = GetOwner();
	if (Owner == nullptr)
	{
		return;
	}
	
	TArray<UMeshComponent*> Meshes;
	Owner->GetComponents<UMeshComponent>(Meshes);
	for (UMeshComponent* Mesh : Meshes)
	{
		if (Mesh != nullptr)
		{
			Mesh->SetRenderCustomDepth(bEnabled);
			Mesh->SetCustomDepthStencilValue(StencilValue);		
		}
	}
}
