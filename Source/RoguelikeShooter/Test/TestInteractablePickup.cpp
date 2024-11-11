#include "TestInteractablePickup.h"

ATestInteractablePickup::ATestInteractablePickup()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ATestInteractablePickup::Pickup(APlayerPawn* Pawn)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "ATestInteractablePickup::Pickup()");

	Destroy();
}
