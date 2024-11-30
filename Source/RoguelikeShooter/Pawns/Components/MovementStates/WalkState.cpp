#include "WalkState.h"

#include "../PlayerMovementComponent.h"
#include "../../PlayerPawn.h"

SWalkState::SWalkState(UPlayerMovementComponent* NewMovementComponent) : SBaseGroundedState(NewMovementComponent) {}

bool SWalkState::UpdateInput()
{
	FPlayerInput& playerInput = MovementComponent->GetPlayerPawn()->GetPlayerInput();

	if (playerInput.bJump)
	{
		playerInput.bJump = false;
		MovementComponent->Jump();
		MovementComponent->SwitchToState(EMovementState::InAir);
		return true;
	}

	if (!(playerInput.bMoveForward || playerInput.bMoveBack || playerInput.bMoveRight || playerInput.bMoveLeft))
	{
		MovementComponent->SwitchToState(EMovementState::Idle);
		return true;
	}

	if (playerInput.bRun && playerInput.bMoveForward && !playerInput.bMoveBack && !playerInput.bMoveRight && !playerInput.bMoveLeft)
	{
		MovementComponent->SwitchToState(EMovementState::Run);
		return true;
	}

	return false;
}

void SWalkState::UpdateVelocity(float DeltaTime)
{
	const FVector targetVelocity = MovementComponent->GetMovementInput() * GetTargetSpeed();
	const FVector projectedVelocity(MovementComponent->Velocity.X, MovementComponent->Velocity.Y, 0.0f);
	const FVector changeVelocity = targetVelocity - projectedVelocity;
	const FVector deltaVelocity = changeVelocity * MovementComponent->GetMovementAttributes().Acceleration * DeltaTime;

	if (changeVelocity.SizeSquared() <= deltaVelocity.SizeSquared()) 
	{
		MovementComponent->Velocity += changeVelocity;
	}
	else 
	{
		MovementComponent->Velocity += deltaVelocity;
	}
}

float SWalkState::GetTargetSpeed()
{
	return MovementComponent->GetMovementAttributes().MaxWalkingSpeed;
}