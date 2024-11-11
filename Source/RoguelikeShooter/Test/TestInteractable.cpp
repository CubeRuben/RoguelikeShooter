#include "TestInteractable.h"


ATestInteractable::ATestInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ATestInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATestInteractable::Interact(APlayerPawn* Pawn)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Interact");
}

void ATestInteractable::StartHovering()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Start Hovering");
}

void ATestInteractable::StopHovering()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Stop Hovering");
}
