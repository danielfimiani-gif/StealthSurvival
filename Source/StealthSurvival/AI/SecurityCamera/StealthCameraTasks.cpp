#include "StealthCameraTasks.h"

#include "StealthSecurityCamera.h"
#include  "StateTreeExecutionContext.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"

namespace 
{
	AStealthSecurityCamera* GetCamera(FStateTreeExecutionContext& Context)
	{
		return Cast<AStealthSecurityCamera>(Context.GetOwner());
	}
}

EStateTreeRunStatus FStealthSTTask_RotateSweep::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	
	AStealthSecurityCamera* Camera = GetCamera(Context);
	if (Camera == nullptr || Camera->GetCameraPivot() == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	Data.InitialYaw = Camera->GetHomeYaw();
	Data.CurrentOffset = 0.f;
	Data.Direction = 1.f;
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStealthSTTask_RotateSweep::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	
	AStealthSecurityCamera* Camera = GetCamera(Context);
	if (Camera == nullptr || Camera->GetCameraPivot() == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (Camera->GetTargetActor() != nullptr)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	
	Data.CurrentOffset += Data.Direction * Data.SweepSpeed * DeltaTime;
	if (Data.CurrentOffset > Data.SweepAngle)
	{
		Data.CurrentOffset = Data.SweepAngle;
		Data.Direction = -1.f;
	}
	else if (Data.CurrentOffset < -Data.SweepAngle)
	{
		Data.CurrentOffset = -Data.SweepAngle;
		Data.Direction = 1.f;
	}
	
	FRotator Rel = Camera->GetCameraPivot()->GetRelativeRotation();
	Rel.Yaw = Data.InitialYaw + Data.CurrentOffset;
	Camera->GetCameraPivot()->SetRelativeRotation(Rel);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStealthSTTask_TrackTarget::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	
	AStealthSecurityCamera* Camera = GetCamera(Context);
	if (Camera == nullptr || Camera->GetCameraPivot() == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AActor* Target = Camera->GetTargetActor();
	if (Target == nullptr)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	
	USceneComponent* Pivot = Camera->GetCameraPivot();
	const FVector PivotLoc = Pivot->GetComponentLocation();
	const FVector TargetLoc = Target->GetActorLocation();
	const FVector DirWorld = (TargetLoc - PivotLoc).GetSafeNormal();
	
	const FVector DirLocal = Camera->GetActorTransform().InverseTransformVectorNoScale(DirWorld);
	FRotator DesiredRel = DirLocal.Rotation();
	
	DesiredRel.Yaw = FMath::Clamp(DesiredRel.Yaw, -Data.MaxYaw, Data.MaxYaw);
	DesiredRel.Pitch = FMath::Clamp(DesiredRel.Pitch, -Data.MaxPitch, Data.MaxPitch);
	DesiredRel.Roll = 0.f;
	
	const FRotator CurrentRel = Pivot->GetRelativeRotation();
	const FRotator NewRel = FMath::RInterpConstantTo(CurrentRel, DesiredRel, DeltaTime, Data.TrackRotationSpeed);
	Pivot->SetRelativeRotation(NewRel);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStealthSTTask_WaitCooldown::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	Data.ElapsedTime = 0.f;
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStealthSTTask_WaitCooldown::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	
	AStealthSecurityCamera* Camera = GetCamera(Context);
	if (Camera == nullptr || Camera->GetCameraPivot() == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (Camera->GetTargetActor() != nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	Data.ElapsedTime += DeltaTime;
	if (Data.ElapsedTime >= Data.CooldownDuration)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	
	return EStateTreeRunStatus::Running;
}
