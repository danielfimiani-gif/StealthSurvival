#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GameplayTagContainer.h"
#include "BTService_OpenNearbyDoors.generated.h"

UCLASS()
class UBTService_OpenNearbyDoors : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_OpenNearbyDoors();
	
protected:
	UPROPERTY(EditAnywhere, Category="SmartObject")
	FGameplayTag DoorTag;
	
	UPROPERTY(EditAnywhere, Category="SmartObject", meta=(ClampMin="0"))
	float SearchRadius = 300.f;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
