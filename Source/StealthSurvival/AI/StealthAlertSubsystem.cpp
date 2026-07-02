#include "StealthAlertSubsystem.h"

void UStealthAlertSubsystem::RaiseAlert(FVector AlertLocation, AActor* AlertInstigator)
{
	OnAlertRaised.Broadcast(AlertLocation, AlertInstigator);
}
