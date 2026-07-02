#include "BTService_OpenNearbyDoors.h"

#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "SmartObjectSubsystem.h"
#include "SmartObjectRequestTypes.h"
#include "SmartObjectComponent.h"
#include "World/StealthDoor.h"

UBTService_OpenNearbyDoors::UBTService_OpenNearbyDoors()
{
      NodeName = "Open Nearby Doors";
      Interval = 0.3f;
      RandomDeviation = 0.05f;
}

void UBTService_OpenNearbyDoors::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
      Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

      AAIController* Controller = OwnerComp.GetAIOwner();
      if (Controller == nullptr)
      {
              return;
      }

      APawn* Pawn = Controller->GetPawn();
      if (Pawn == nullptr)
      {
              return;
      }

      USmartObjectSubsystem* SOSubsystem = USmartObjectSubsystem::GetCurrent(Controller->GetWorld());
      if (SOSubsystem == nullptr)
      {
              return;
      }

      const FVector Origin = Pawn->GetActorLocation();
      const FBox QueryBox = FBox::BuildAABB(Origin, FVector(SearchRadius));

      FSmartObjectRequestFilter Filter;
      FGameplayTagContainer RequiredTag;
      RequiredTag.AddTag(DoorTag);
      Filter.ActivityRequirements = FGameplayTagQuery::MakeQuery_MatchAllTags(RequiredTag);

      const FSmartObjectRequest Request(QueryBox, Filter);

      const FSmartObjectRequestResult Result = SOSubsystem->FindSmartObject(Request, Pawn);
      if (!Result.IsValid())
      {
              return;
      }

      const FSmartObjectClaimHandle ClaimHandle = SOSubsystem->MarkSlotAsClaimed(
              Result.SlotHandle,
              ESmartObjectClaimPriority::Normal
      );
      if (!ClaimHandle.IsValid())
      {
              return;
      }

      if (USmartObjectComponent* SOComp = SOSubsystem->GetSmartObjectComponent(ClaimHandle))
      {
              if (AStealthDoor* Door = Cast<AStealthDoor>(SOComp->GetOwner()))
              {
                      Door->OpenForAI();
              }
      }

      SOSubsystem->Release(ClaimHandle);
}