#include "EQSContext_InvestigateLocation.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

void UEQSContext_InvestigateLocation::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AActor* Owner = Cast<AActor>(QueryInstance.Owner.Get());
	AAIController* AICon = Cast<AAIController>(Owner);
	if (AICon == nullptr)
	{
		if (const APawn* Pawn = Cast<APawn>(Owner))
		{
			AICon = Cast<AAIController>(Pawn->GetController());
		}
	}
	
	if (AICon == nullptr)
	{
		return;
	}
	
	const UBlackboardComponent* BB = AICon->GetBlackboardComponent();
	if (BB == nullptr)
	{
		return;
	}
	
	const FVector Location = BB->GetValueAsVector(TEXT("InvestigateLocation"));
	
	UEnvQueryItemType_Point::SetContextHelper(ContextData, Location);
}
