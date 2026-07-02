#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateAlertState.generated.h"

UCLASS()
class UBTService_UpdateAlertState : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdateAlertState();

protected:
	UPROPERTY(EditAnywhere, Category="Alert")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category="Alert")
	FBlackboardKeySelector HasLineOfSightKey;

	UPROPERTY(EditAnywhere, Category="Alert")
	FBlackboardKeySelector InvestigateLocationKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};