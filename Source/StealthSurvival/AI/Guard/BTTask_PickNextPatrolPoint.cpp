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
	
	int32 Index = Blackboard->GetValueAsInt(PatrolIndexKey.SelectedKeyName);
	if (!Points.IsValidIndex(Index))
	{
		Index = 0;
	}
	
	AActor* TargetPoint = Points[Index];
	if (TargetPoint == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	Blackboard->SetValueAsVector(BlackboardKey.SelectedKeyName, TargetPoint->GetActorLocation());
	
	const int32 NewIndex = (Index + 1) % Points.Num();
	Blackboard->SetValueAsInt(PatrolIndexKey.SelectedKeyName, NewIndex);
	
	return EBTNodeResult::Succeeded;
}
