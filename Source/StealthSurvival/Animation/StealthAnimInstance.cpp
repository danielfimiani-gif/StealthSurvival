
#include "StealthAnimInstance.h"
#include "StealthSurvivalCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UStealthAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		OwningCharacter = Cast<ACharacter>(OwningPawn);
		StealthCharacter = Cast<AStealthSurvivalCharacter>(OwningPawn);
	}
}

void UStealthAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (OwningCharacter.IsValid())
	{
		if (const UCharacterMovementComponent* MovementComp = OwningCharacter->GetCharacterMovement())
		{
			const FVector Velocity = MovementComp->Velocity;
			Speed = Velocity.Size2D();
			bIsInAir = MovementComp->IsFalling();
			bIsCrouched = MovementComp->IsCrouching();
			Direction= UKismetAnimationLibrary::CalculateDirection(Velocity, OwningCharacter->GetActorRotation());
		}

		bIsSprinting = StealthCharacter.IsValid() ? StealthCharacter->IsSprinting() : false;
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
