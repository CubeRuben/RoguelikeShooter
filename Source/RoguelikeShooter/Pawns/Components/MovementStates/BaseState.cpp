#include "BaseState.h"

#include "../PlayerMovementComponent.h"
#include "../../PlayerPawn.h"

SBaseState::SBaseState(UPlayerMovementComponent* NewMovementComponent)
{
	MovementComponent = NewMovementComponent;
}