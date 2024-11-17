#include "RunState.h"

#include "../PlayerMovementComponent.h"
#include "../../PlayerPawn.h"

SRunState::SRunState(UPlayerMovementComponent* NewMovementComponent) : SWalkState(NewMovementComponent) {}

bool SRunState::UpdateInput()
{
	FPlayerInput& playerInput = MovementComponent->GetPlayerPawn()->GetPlayerInput();

	// Handle input
	if (playerInput.bJump)
	{
		playerInput.bJump = false;
		MovementComponent->Jump();
		MovementComponent->SwitchToState(EMovementState::InAir);
		return true;
	}

	if (!(playerInput.bMoveForward && playerInput.bRun) || playerInput.bMoveBack || playerInput.bMoveRight || playerInput.bMoveLeft)
	{
		MovementComponent->SwitchToState(EMovementState::Idle);
		return true;
	}

	return false;
}

float SRunState::GetTargetSpeed()
{
	return MovementComponent->GetMovementAttributes().MaxRunningSpeed;
}
