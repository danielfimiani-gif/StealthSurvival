#pragma once

#include  "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "StealthAnimInstance.generated.h" 

class AStealthSurvivalCharacter;

UCLASS()
class UStealthAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Stealth|Anim")
	float Speed = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category="Stealth|Anim")
	bool bIsInAir = false;
	
	UPROPERTY(BlueprintReadOnly, Category="Stealth|Anim")
	bool bIsCrouched = false;
	
	UPROPERTY(BlueprintReadOnly, Category="Stealth|Anim")
	bool bIsSprinting = false;
	
	UPROPERTY(BlueprintReadOnly, Category="Stealth|Anim")
	bool bShouldMove = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Stealth|Anim", meta=(ClampMin="0"))
	float MinMoveSpeed = 3.f;
	
	UPROPERTY(BlueprintReadOnly, Category="Stealth|Anim")
	float Direction = 0.f;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	
	TWeakObjectPtr<class AStealthSurvivalCharacter> StealthCharacter;
};
