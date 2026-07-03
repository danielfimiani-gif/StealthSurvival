#include "BTTask_PickNextPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Guard/StealthGuardCharacter.h"

UBTTask_PickNextPatrolPoint::UBTTask_PickNextPatrolPoint()
{
	NodeName = TEXT("Pick Next Patrol Point");
}

EBTNodeResult::Type UBTTask_PickNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	AStealthGuardCharacter* Guard = Cast<AStealthGuardCharacter>(AIController->GetPawn());
	if (Guard == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	const TArray<TObjectPtr<AActor>>& Points = Guard->GetPatrolPoints();
	if (Points.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	int32 VirtualIndex = Blackboard->GetValueAsInt(PatrolIndexKey.SelectedKeyName);
	const int32 Num = Points.Num();

	int32 ActualIndex = 0;
	int32 NextVirtualIndex = 0;
	if (Num > 1)
	{
		const int32 Cycle = 2 * Num - 2;
		const int32 Wrapped = ((VirtualIndex % Cycle) + Cycle) % Cycle;
		ActualIndex = (Wrapped < Num) ? Wrapped : (Cycle - Wrapped);
		NextVirtualIndex = Wrapped + 1;
	}

	AActor* TargetPoint = Points[ActualIndex];
	if (TargetPoint == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Blackboard->SetValueAsVector(BlackboardKey.SelectedKeyName, TargetPoint->GetActorLocation());
	Blackboard->SetValueAsInt(PatrolIndexKey.SelectedKeyName, NextVirtualIndex);

	return EBTNodeResult::Succeeded;
}
