#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StealthCameraTasks.generated.h"

USTRUCT()
struct FStealthSTTask_RotateSweepInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Parameter", meta = (ClampMin = "0", ClampMax = "100"))
	float SweepAngle = 45.f;
	
	UPROPERTY(EditAnywhere, Category="Parameter", meta = (ClampMin = "0"))
	float SweepSpeed = 30.f;
	
	float InitialYaw = 0.f;
	float CurrentOffset = 0.f;
	float Direction = 1.f;
};

USTRUCT(meta = (DisplayName = "Camera Rotate Sweep"))
struct FStealthSTTask_RotateSweep : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FStealthSTTask_RotateSweepInstanceData;
	virtual const UStruct* GetInstanceDataType() const override {return FInstanceDataType::StaticStruct();}
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT()
struct FStealthSTTask_TrackTargetInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Parameter", meta = (ClampMin = "0"))
	float TrackRotationSpeed = 120.f;
	
	UPROPERTY(EditAnywhere, Category="Parameter", meta = (ClampMin = "0", ClampMax = "180"))
	float MaxYaw = 80.f;
	
	UPROPERTY(EditAnywhere, Category="Parameter", meta = (ClampMin = "0", ClampMax = "89"))
	float MaxPitch = 20.f;
};

USTRUCT(meta = (DisplayName = "Camera Track Target"))
struct FStealthSTTask_TrackTarget : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	using FInstanceDataType = FStealthSTTask_TrackTargetInstanceData;
	virtual const UStruct* GetInstanceDataType() const override {return FInstanceDataType::StaticStruct();}
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT()
struct FStealthSTTask_WaitCooldownInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Parameter", meta = (ClampMin = "0"))
	float CooldownDuration = 3.f;
	
	float ElapsedTime = 0.f;
};

USTRUCT(meta = (DisplayName = "Camera Wait Cooldown"))
struct FStealthSTTask_WaitCooldown : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FStealthSTTask_WaitCooldownInstanceData;
	virtual const UStruct* GetInstanceDataType() const override {return FInstanceDataType::StaticStruct();}
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
