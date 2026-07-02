#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearBlackboardValue.generated.h"

UCLASS()

class UBTTask_ClearBlackboardValue : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ClearBlackboardValue();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector BlackboardKey;
};
