// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthSurvivalCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "StealthSurvival.h"
#include "Perception/AISense_Hearing.h"
#include "Guard/StealthGuardCharacter.h"
#include "Items/StealthThrowable.h"
#include "GameFramework/PlayerController.h"
#include "Components/MeshComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

AStealthSurvivalCharacter::AStealthSurvivalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->bOrientRotationToMovement = false;
	MovementComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	MovementComp->JumpZVelocity = 500.f;
	MovementComp->AirControl = 0.35f;
	MovementComp->MaxWalkSpeed = WalkSpeed;
	MovementComp->MinAnalogWalkSpeed = 20.f;
	MovementComp->BrakingDecelerationWalking = 2000.f;
	MovementComp->BrakingDecelerationFalling = 1500.0f;
	MovementComp->NavAgentProps.bCanCrouch = true;
	MovementComp->MaxWalkSpeedCrouched = CrouchSpeed;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 900.0f;
	CameraBoom->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bDoCollisionTest = false;
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AStealthSurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		for (const TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
		{
			if (Ability != nullptr)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, -1, this));
			}
		}
	}
}

void AStealthSurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStealthSurvivalCharacter::Move);
		
		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AStealthSurvivalCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AStealthSurvivalCharacter::StopSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AStealthSurvivalCharacter::StopSprint);
		
		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AStealthSurvivalCharacter::ToggleCrouch);
		
		// Takedown
		EnhancedInputComponent->BindAction(TakeDownAction, ETriggerEvent::Started,this, &AStealthSurvivalCharacter::ExecuteTakeDown);
		
		// Throw
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started,this, &AStealthSurvivalCharacter::ExecuteThrow);
	
		// Hide
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AStealthSurvivalCharacter::Interact);
		
		// Invisible
		EnhancedInputComponent->BindAction(InvisibilityAction, ETriggerEvent::Started, this, &AStealthSurvivalCharacter::ToggleInvisibility);
	}
	else
	{
		UE_LOG(LogStealthSurvival, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AStealthSurvivalCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AStealthSurvivalCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AStealthSurvivalCharacter::DoMove(float Right, float Forward)
{
	if (GetController() == nullptr)
	{
		return;
	}
	
	const FRotator CamYaw(0.f, CameraBoom->GetRelativeRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(CamYaw).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(CamYaw).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);
}

void AStealthSurvivalCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AStealthSurvivalCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AStealthSurvivalCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AStealthSurvivalCharacter::StartSprint()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	
	bIsSprinting = true;
	UpdateMovementSpeed();
}

void AStealthSurvivalCharacter::StopSprint()
{
	bIsSprinting = false;
	UpdateMovementSpeed();
}

void AStealthSurvivalCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		bIsSprinting = false;
		Crouch();
	}
	
	UpdateMovementSpeed();
}

void AStealthSurvivalCharacter::ToggleInvisibility()
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}
	
	FGameplayTagContainer InvisTags;
	InvisTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Invisibility")));
	
	if (bIsInvisible)
	{
		AbilitySystemComponent->CancelAbilities(&InvisTags);
	}
	else
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(InvisTags);
	}
}

void AStealthSurvivalCharacter::UpdateMovementSpeed()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp == nullptr) return;
	
	if (bIsSprinting)
		MovementComp->MaxWalkSpeed = RunSpeed;
	else
		MovementComp->MaxWalkSpeed = WalkSpeed;
	
	MovementComp->MaxWalkSpeedCrouched = CrouchSpeed;
}

float AStealthSurvivalCharacter::GetCurrentNoiseRange() const
{
	if (bIsCrouched) return CrouchNoiseRange;
	if (bIsSprinting) return RunNoiseRange;
	return WalkNoiseRange;
}

void AStealthSurvivalCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AimAtCursor();
	UpdateCameraPan(DeltaSeconds);
	UpdateCameraOcclusion();
	
	NoiseEmissionTimer += DeltaSeconds;
	if (NoiseEmissionTimer < NoiseEmissionInterval)
	{
		return;
	}
	NoiseEmissionTimer = 0.f;

	if (bIsCrouched)
	{
		return;
	}

	const float CurrentSpeed = GetVelocity().Size2D();
	if (CurrentSpeed < 5.f)
	{
		return;
	}
	
	const float NoiseRange = GetCurrentNoiseRange();
	UAISense_Hearing::ReportNoiseEvent(
		GetWorld(),
		GetActorLocation(),
		1.0f,
		this,
		NoiseRange
	);
}

void AStealthSurvivalCharacter::ExecuteTakeDown()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + GetActorForwardVector() * TakeDownTraceDistance;
	
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	const bool bHit = World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn, Params);
	if (!bHit)
	{
		return;
	}
	
	AStealthGuardCharacter* Guard = Cast<AStealthGuardCharacter>(Hit.GetActor());
	if (Guard == nullptr)
	{
		return;
	}
	
	FVector GuardToPlayer = GetActorLocation() - Guard->GetActorLocation();
	GuardToPlayer.Z = 0.f;
	GuardToPlayer.Normalize();
	
	FVector GuardForward = Guard->GetActorForwardVector();
	GuardForward.Z = 0.f;
	GuardForward.Normalize();
	
	const float DotProduct = FVector::DotProduct(GuardForward, GuardToPlayer);
	
	if (DotProduct > TakeDownRearDotThreshold)
	{
		return;
	}
	
	Guard->Die();
}

void AStealthSurvivalCharacter::ExecuteThrow()
{
	if (ThrowableClass == nullptr)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	
	const FRotator ControlRot = GetControlRotation();
	const FRotator YawOnly(0.f, ControlRot.Yaw, 0.f);
	const FVector SpawnLocation = GetActorLocation() + YawOnly.RotateVector(ThrowSpawnOffset);
	
	const FRotator LaunchRot(ControlRot.Pitch + ThrowPitchOffset, ControlRot.Yaw, 0.f);
	const FVector LaunchVelocity = LaunchRot.Vector() * ThrowSpeed;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	
	AStealthThrowable* Throwable = World->SpawnActor<AStealthThrowable>(ThrowableClass, SpawnLocation, LaunchRot, SpawnParams);
	if (Throwable != nullptr)
	{
		Throwable->Launch(LaunchVelocity);
	}
}

UAISense_Sight::EVisibilityResult AStealthSurvivalCharacter::CanBeSeenFrom(
	const FCanBeSeenFromContext& Context, 
	FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, 
	int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, 
	int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
	if (bIsHidden || bIsInvisible)
	{
		return UAISense_Sight::EVisibilityResult::NotVisible;
	}
	
	OutNumberOfAsyncLosCheckRequested = 0;
	OutSightStrength = 1.f;
	
	const FVector TargetLocation = GetActorLocation();
	
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AILineOfSight), true, Context.IgnoreActor);
	Params.AddIgnoredActor(this);
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, Context.ObserverLocation, TargetLocation, ECC_Visibility, Params
	);
	
	OutNumberOfLoSChecksPerformed = 1;
	
	if (bHit)
	{
		return UAISense_Sight::EVisibilityResult::NotVisible;
	}
	
	if (IsInCover())
	{
		return UAISense_Sight::EVisibilityResult::NotVisible;
	}
	
	OutSeenLocation = TargetLocation;
	return UAISense_Sight::EVisibilityResult::Visible;
}

void AStealthSurvivalCharacter::AimAtCursor()
{
	if (bIsHidden)
	{
		return;
	}
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC == nullptr)
	{
		return;
	}
	
	FVector WorldOrigin, WorldDirection;
	if (!PC->DeprojectMousePositionToWorld(WorldOrigin, WorldDirection))
	{
		return;
	}
	
	if (FMath::IsNearlyZero(WorldDirection.Z))
	{
		return;
	}
	
	const FVector PlayerLoc = GetActorLocation();
	const float T = (PlayerLoc.Z - WorldOrigin.Z) / WorldDirection.Z;
	if (T < 0.f)
	{
		return;
	}
	
	const FVector CursorWorld = WorldOrigin + WorldDirection * T;
	
	FVector ToCursor = CursorWorld - PlayerLoc;
	ToCursor.Z = 0.f;
	if (ToCursor.IsNearlyZero())
	{
		return;
	}
	
	SetActorRotation(FRotator(0.f, ToCursor.Rotation().Yaw, 0.f));
}

void AStealthSurvivalCharacter::UpdateCameraPan(float DeltaSeconds)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC == nullptr) return;

	const bool bIsMoving = !GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero();

    if (bIsMoving)
    {
        CameraPanOffset = FMath::VInterpTo(CameraPanOffset, FVector::ZeroVector, DeltaSeconds, CameraReturnSpeed);
    }
    else
    {
        float MouseX, MouseY;
        int32 ViewX, ViewY;
        PC->GetViewportSize(ViewX, ViewY);

        if (PC->GetMousePosition(MouseX, MouseY) && ViewX > 0 && ViewY > 0)
        {
            const float EdgeX = ViewX * EdgeScrollZone;
            const float EdgeY = ViewY * EdgeScrollZone;

            FVector2D PanDir = FVector2D::ZeroVector;
            if (MouseX <= EdgeX)              PanDir.X = -1.f;
            else if (MouseX >= ViewX - EdgeX) PanDir.X =  1.f;
            if (MouseY <= EdgeY)              PanDir.Y = -1.f;
            else if (MouseY >= ViewY - EdgeY) PanDir.Y =  1.f;

            if (!PanDir.IsZero())
            {
                const FRotator CamYaw(0.f, CameraBoom->GetRelativeRotation().Yaw,0.f);
                const FVector Fwd   = FRotationMatrix(CamYaw).GetUnitAxis(EAxis::X);
                const FVector Right = FRotationMatrix(CamYaw).GetUnitAxis(EAxis::Y);

                FVector WorldPan = (Fwd * -PanDir.Y) + (Right * PanDir.X);
                WorldPan.Normalize();

                CameraPanOffset += WorldPan * CameraPanSpeed * DeltaSeconds;
                CameraPanOffset = CameraPanOffset.GetClampedToMaxSize(CameraPanMaxDistance);
            }
        }
    }

    CameraBoom->TargetOffset = CameraPanOffset;
}

void AStealthSurvivalCharacter::Interact()
{
	if (bIsHidden)
	{
		ExitHidingSpot();
		return;
	}
	
	if (InteractableInRange)
	{
		InteractableInRange->Interact(this);
	}
}

void AStealthSurvivalCharacter::EnterHidingSpot(AActor* Spot, const FTransform& SlotTransform)
{
	if (bIsHidden)
	{
		return;
	}
	
	bIsHidden = true;
	CurrentHidingSpot = Spot;
	PreHideTransform = GetActorTransform();
	
	SetActorLocation(SlotTransform.GetLocation());
	SetActorRotation(SlotTransform.GetRotation());
	
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetVisibility(false, true);
}

void AStealthSurvivalCharacter::ExitHidingSpot()
{
	if (!bIsHidden)
	{
		return;
	}
	
	bIsHidden = false;
	CurrentHidingSpot = nullptr;
	
	SetActorLocation(PreHideTransform.GetLocation());
	SetActorRotation(PreHideTransform.GetRotation());
	
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetMesh()->SetVisibility(true, true);
}

UAbilitySystemComponent* AStealthSurvivalCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AStealthSurvivalCharacter::UpdateCameraOcclusion()
{
	if (FollowCamera == nullptr)
	{
		return;
	}
	
	const FVector Start = FollowCamera->GetComponentLocation();
	const FVector End = GetActorLocation();
	
	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (CurrentHidingSpot != nullptr)
	{
		Params.AddIgnoredActor(CurrentHidingSpot);
	}
	
	GetWorld()->SweepMultiByChannel(
		Hits, Start, End, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(OcclusionProbeRadius),Params
	);
	
	TSet<TObjectPtr<AActor>> CurrenOccluders;
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor == nullptr) continue;
		if (HitActor->IsA<APawn>()) continue;
		
		CurrenOccluders.Add(HitActor);
		if (!HiddenOccluders.Contains(HitActor))
		{
			SetOccluderFade(HitActor, OccluderFadeOpacity);
			HiddenOccluders.Add(HitActor);
		}
	}
		
	for (auto It = HiddenOccluders.CreateIterator(); It; ++It)
	{
		if (!CurrenOccluders.Contains(*It))
		{
			if (*It)
			{
				SetOccluderFade(*It,  1.f);
			}
			It.RemoveCurrent();
		}
	}
}

void AStealthSurvivalCharacter::SetOccluderFade(AActor* Occluder, float FadeValue)
{
	if (Occluder == nullptr)
	{
		return;
	}
	
	TArray<UMeshComponent*> Meshes;
	Occluder->GetComponents<UMeshComponent>(Meshes);
	for (UMeshComponent* UMesh : Meshes)
	{
		if (UMesh != nullptr)
		{
			UMesh->SetScalarParameterValueOnMaterials(OccluderFadeParameter, FadeValue);
		}
	}
}

void AStealthSurvivalCharacter::SetInteractableInRange(const TScriptInterface<IInteractable>& Interactable)
{
	if (InteractableInRange.GetObject() == Interactable.GetObject())
	{
		return;
	}
	
	if (InteractableInRange.GetObject() != nullptr)
	{
		InteractableInRange->OnEndFocus();
	}
	
	InteractableInRange = Interactable;
	
	if (InteractableInRange.GetObject() != nullptr)
	{
		InteractableInRange->OnBeginFocus();
	}
}

void AStealthSurvivalCharacter::ClearInteractableInRange(const TScriptInterface<IInteractable>& Interactable)
{
	if (InteractableInRange.GetObject() != Interactable.GetObject())
	{
		return;
	}
	
	InteractableInRange->OnEndFocus();
	InteractableInRange = nullptr;
}

void AStealthSurvivalCharacter::SetInvisible(bool bNewInvisible)
{
	bIsInvisible = bNewInvisible;
}
