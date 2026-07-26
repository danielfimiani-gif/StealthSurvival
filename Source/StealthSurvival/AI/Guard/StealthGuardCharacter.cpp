#include "StealthGuardCharacter.h"

#include "BrainComponent.h"
#include "StealthAIController.h" 
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

AStealthGuardCharacter::AStealthGuardCharacter()
{
	AIControllerClass = AStealthAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	TeamId = FGenericTeamId(1);
	
	VisionConeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisionCone"));
	VisionConeMesh->SetupAttachment(RootComponent);
	VisionConeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisionConeMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -85.0f));
	VisionConeMesh->SetCastShadow(false);

	TakedownPrompt = CreateDefaultSubobject<UWidgetComponent>(TEXT("TakedownPrompt"));
	TakedownPrompt->SetupAttachment(RootComponent);
	TakedownPrompt->SetWidgetSpace(EWidgetSpace::Screen);
	TakedownPrompt->SetDrawSize(FVector2D(80.f, 80.f));
	TakedownPrompt->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	TakedownPrompt->SetVisibility(false);

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = GetPatrolSpeed();
	}
}

void AStealthGuardCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (VisionConeMesh != nullptr)
	{
		VisionConeMesh->SetScalarParameterValueOnMaterials(TEXT("Halfangle"), GetSightHalfAngle());
	}
}

void AStealthGuardCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = GetPatrolSpeed();
	}
}

void AStealthGuardCharacter::Die()
{
	if (bIsDead)
	{
		return;
	}
	
	bIsDead = true;

	if (VisionConeMesh != nullptr)
	{
		VisionConeMesh->SetVisibility(false);
	}

	AStealthAIController* AIController = Cast<AStealthAIController>(GetController());
	if (AIController != nullptr)
	{
		if (UBrainComponent* Brain = AIController->GetBrainComponent())
		{
			Brain->StopLogic(TEXT("Killed by takedown"));
		}
		AIController->UnPossess();
		AIController->Destroy();
	}
	
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->DisableMovement();
		Movement->StopMovementImmediately();
	}
	
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
		MeshComp->SetSimulatePhysics(true);
	}

	SetLifeSpan(DeathLifespan);
}

void AStealthGuardCharacter::SetTakedownAvailable(bool bAvailable)
{
	if (TakedownPrompt != nullptr)
	{
		TakedownPrompt->SetVisibility(bAvailable);
	}
}

void AStealthGuardCharacter::SetAlertState(EStealthAlertState NewState)
{
	if (AlertState == NewState)
	{
		return;
	}			
	
	AlertState = NewState;

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = (NewState == EStealthAlertState::Alerted) ? GetChaseSpeed() : GetPatrolSpeed();
	}

	if (VisionConeMesh == nullptr)
	{
		return;
	}
	
	const FLinearColor Color = GetAlertColor(NewState);
	VisionConeMesh->SetVectorParameterValueOnMaterials(TEXT("ConeColor"), FVector(Color.R, Color.G, Color.B));
}

FLinearColor AStealthGuardCharacter::GetAlertColor(EStealthAlertState State) const
{
	const FLinearColor Unaware = GuardConfig ? GuardConfig->UnawareColor : UnawareColor;
	const FLinearColor Suspicious = GuardConfig ? GuardConfig->SuspiciousColor : SuspiciousColor;
	const FLinearColor Alerted = GuardConfig ? GuardConfig->AlertedColor : AlertedColor;

	switch (State)
	{
		case EStealthAlertState::Suspicious: return Suspicious;
		case EStealthAlertState::Alerted: return Alerted;
		default: return Unaware;
	}
}

