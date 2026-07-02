
#include "StealthAnimInstance.h"
#include "StealthSurvivalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UStealthAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		StealthCharacter = Cast<AStealthSurvivalCharacter>(OwningPawn);
	}
}

void UStealthAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (StealthCharacter.IsValid())
	{
		if (const UCharacterMovementComponent* MovementComp = StealthCharacter->GetCharacterMovement())
		{
			const FVector Velocity = MovementComp->Velocity;
			Speed = Velocity.Size2D();
			bIsInAir = MovementComp->IsFalling();
			bIsCrouched = MovementComp->IsCrouching();
			Direction= UKismetAnimationLibrary::CalculateDirection(Velocity, StealthCharacter->GetActorRotation());
		}
		
		bIsSprinting = StealthCharacter->IsSprinting();
		bShouldMove = Speed > MinMoveSpeed;
	}
	else {
		Speed = 0.f;
		bIsInAir = false;
		bIsCrouched = false;
		bIsSprinting = false;
		bShouldMove = false;
	}
}
