// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Perception/AISightTargetInterface.h"
#include "Interaction/Interactable.h"
#include "StealthSurvivalCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UAIPerceptionStimuliSourceComponent;
class AStealthGuardCharacter;
class UAbilitySystemComponent;
class UGameplayAbility;
class UAnimMontage;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AStealthSurvivalCharacter : public ACharacter, public IAISightTargetInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;

protected:
	/** Jump to Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* CrouchAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* TakeDownAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ThrowAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InvisibilityAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SmokeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Locomotion", meta=(ClampMin="0"))
	float WalkSpeed = 350.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Locomotion", meta=(ClampMin="0"))
	float RunSpeed = 650.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Locomotion", meta=(ClampMin="0"))
	float CrouchSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Noise", meta=(ClampMin="0"))
	float WalkNoiseRange = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Noise", meta=(ClampMin="0"))
	float RunNoiseRange = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Combat", meta=(ClampMin="0"))
	float TakeDownTraceDistance = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stealth|Combat", meta=(ClampMin="0"))
	float TakeDownRearDotThreshold = -0.3f;

	UPROPERTY(EditDefaultsOnly, Category="Stealth|Combat")
	TObjectPtr<UAnimMontage> TakeDownMontage;

public:
	/** Constructor */
	AStealthSurvivalCharacter();	
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(
		const FCanBeSeenFromContext& Context, 
		FVector& OutSeenLocation, 
		int32& OutNumberOfLoSChecksPerformed, 
		int32& OutNumberOfAsyncLosCheckRequested, 
		float& OutSightStrength, 
		int32* UserData = nullptr, 
		const FOnPendingVisibilityQueryProcessedDelegate* Delegate = nullptr
	) override;
	
	void AddCoverSource() { ++CoverSourceCount; }
	void RemoveCoverSource() { CoverSourceCount = FMath::Max(0, CoverSourceCount - 1); }
	
	void AddSmokeSource() { ++SmokeSourceCount; }	
	void RemoveSmokeSource() { SmokeSourceCount = FMath::Max(0, SmokeSourceCount - 1); }
	
	void SetInteractableInRange(const TScriptInterface<IInteractable>& Interactable); 
	void ClearInteractableInRange(const TScriptInterface<IInteractable>& Interactable);
	void EnterHidingSpot(AActor* Spot, const FTransform& SlotTransform);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void SetInvisible(bool bNewInvisible);
	
	UFUNCTION(BlueprintPure, Category="Stealth|Hide")
	bool IsHidden() { return bIsHidden; }
	
	UFUNCTION(BlueprintPure, Category="Stealth|Cover")
	bool IsInCover() const {return CoverSourceCount > 0 && bIsCrouched; }

	UFUNCTION(BlueprintPure, Category="Stealth|Cover")
	bool IsInvisible() const { return bIsInvisible; }
	
	UFUNCTION(BlueprintPure, Category="Stealth|Cover")
	bool IsInSmoke() const { return SmokeSourceCount > 0; }
	
protected:
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void ExecuteTakeDown();
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void StartSprint();
	void StopSprint();
	void ToggleCrouch();
	void ToggleInvisibility();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

public:
	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	
	UFUNCTION(BlueprintPure, Category="Stealth")
	bool IsSprinting() const { return bIsSprinting; }
	
	UFUNCTION(BlueprintPure, Category="Stealth|Noise")
	float GetCurrentNoiseRange() const;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	void UpdateMovementSpeed();
	
	void AimAtCursor();
	
	void UpdateCameraPan(float DeltaSeconds);
	FVector CameraPanOffset = FVector::ZeroVector;
	
	void UpdateCameraOcclusion();
	
	void SetOccluderFade(AActor* Occluder, float FadeValue);
	
	void ActivateThrow();
	
	void ActivateSmoke();
	
	void TryActivateAbilityByTag(const FName& TagName);
	
	UPROPERTY(EditDefaultsOnly, Category="Camera|Occlusion", meta=(ClampMin="0", ClampMax="1"))
	float OccluderFadeOpacity = 0.3f;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera|Occlusion")
	FName OccluderFadeParameter = "Fade";
	
	UPROPERTY(EditDefaultsOnly, Category="Camera|Occlusion", meta=(ClampMin="0"))
	float OcclusionProbeRadius = 25.f;
	
	UPROPERTY()
	TSet<TObjectPtr<AActor>> HiddenOccluders;
	
	UPROPERTY()
	TObjectPtr<AActor> CurrentHidingSpot;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera|Pan", meta=(ClampMin="0"))
	float CameraPanSpeed = 1500.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera|Pan", meta=(ClampMin="0"))
	float CameraPanMaxDistance = 1200.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera|Pan", meta=(ClampMin="0", ClampMax="0.5"))
	float EdgeScrollZone = 0.05f;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera|Pan", meta=(ClampMin="0"))
	float CameraReturnSpeed = 8.f;
	
	bool bIsSprinting = false;
	
	float NoiseEmissionTimer = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Stealth|Noise", meta=(ClampMin="0.5"))
	float NoiseEmissionInterval = 0.2f;
	
	int32 CoverSourceCount = 0;
	int32 SmokeSourceCount = 0;
	
	void Interact();
	void ExitHidingSpot();

	AStealthGuardCharacter* FindTakedownTarget() const;
	TWeakObjectPtr<class AStealthGuardCharacter> CurrentTakedownTarget;
	
	bool bIsHidden = false;
	FTransform PreHideTransform;
	
	UPROPERTY()
	TScriptInterface<IInteractable> InteractableInRange;
	
	bool bIsInvisible = false;
};

