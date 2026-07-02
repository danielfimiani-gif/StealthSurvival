#include "BTService_UpdateAlertState.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Guard/StealthGuardCharacter.h"

UBTService_UpdateAlertState::UBTService_UpdateAlertState()
{
	NodeName = "Update Alert State";
	Interval = 0.2f;
	RandomDeviation = 0.05f;
}

void UBTService_UpdateAlertState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
	{
		return;
	}

	AStealthGuardCharacter* Guard = Cast<AStealthGuardCharacter>(Controller->GetPawn());
	if (Guard == nullptr)
	{
		return;
	}

	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	if (BlackBoard == nullptr)
	{
		return;
	}

	const bool bHasTarget = BlackBoard->GetValueAsObject(TargetActorKey.SelectedKeyName) != nullptr;
	const bool bHasLos = BlackBoard->GetValueAsBool(HasLineOfSightKey.SelectedKeyName);
	const bool bInvestigating = BlackBoard->IsVectorValueSet(InvestigateLocationKey.SelectedKeyName);
	
	EStealthAlertState NewState = EStealthAlertState::Unaware;
	if (bHasTarget && bHasLos)
	{
		NewState = EStealthAlertState::Alerted;
	}
	else if (bHasTarget || bInvestigating)
	{
		NewState = EStealthAlertState::Suspicious;
	}
	
	Guard->SetAlertState(NewState);
}