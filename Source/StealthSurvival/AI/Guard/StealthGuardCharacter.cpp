#include "StealthGuardCharacter.h"

#include "BrainComponent.h"
#include "StealthAIController.h" 
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"

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

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = PatrolSpeed;
	}
}

void AStealthGuardCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (VisionConeMesh != nullptr)
	{
		VisionConeMesh->SetScalarParameterValueOnMaterials(TEXT("Halfangle"), SightHalfAngle);
	}
}

void AStealthGuardCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = PatrolSpeed;
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

void AStealthGuardCharacter::SetAlertState(EStealthAlertState NewState)
{
	if (AlertState == NewState)
	{
		return;
	}			
	
	AlertState = NewState;

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = (NewState == EStealthAlertState::Alerted) ? ChaseSpeed : PatrolSpeed;
	}

	if (VisionConeMesh == nullptr)
	{
		return;
	}
	
	FLinearColor Color = UnawareColor;
	switch (NewState)
	{
	case EStealthAlertState::Suspicious:
		Color = SuspiciousColor;
		break;
	case EStealthAlertState::Alerted:
		Color = AlertedColor;
		break;
	default:
		break;
	}
	
	VisionConeMesh->SetVectorParameterValueOnMaterials(TEXT("ConeColor"), FVector(Color.R, Color.G, Color.B));
}
