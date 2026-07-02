#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PickNextPatrolPoint.generated.h"

UCLASS()
class UBTTask_PickNextPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_PickNextPatrolPoint();
	
protected:
	UPROPERTY(EditAnywhere, Category="Patrol")
	FBlackboardKeySelector PatrolIndexKey;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
