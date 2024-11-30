#include "IdleState.h"

#include "../PlayerMovementComponent.h"
#include "../../PlayerPawn.h"

SIdleState::SIdleState(UPlayerMovementComponent* NewMovementComponent) : SBaseGroundedState(NewMovementComponent) {}

bool SIdleState::UpdateInput()
{
	FPlayerInput& playerInput = MovementComponent->GetPlayerPawn()->GetPlayerInput();

	if (playerInput.bJump)
	{
		playerInput.bJump = false;
		MovementComponent->Jump();
		MovementComponent->SwitchToState(EMovementState::InAir);
		return true;
	}

	if (playerInput.bMoveForward || playerInput.bMoveBack || playerInput.bMoveRight || playerInput.bMoveLeft)
	{
		if (playerInput.bRun && playerInput.bMoveForward && !playerInput.bMoveBack && !playerInput.bMoveRight && !playerInput.bMoveLeft)
		{
			MovementComponent->SwitchToState(EMovementState::Run);
		}
		else 
		{
			MovementComponent->SwitchToState(EMovementState::Walk);
		}
		return true;
	}

	return false;
}

void SIdleState::UpdateVelocity(float DeltaTime)
{
	const FVector deltaVelocity = MovementComponent->Velocity * MovementComponent->GetMovementAttributes().StoppingAcceleration * DeltaTime;

	//const float dot = FVector::DotProduct(MovementComponent->Velocity.GetSafeNormal2D(), deltaVelocity.GetSafeNormal2D());
	//
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(dot));

	if (deltaVelocity.SizeSquared() < 1.0f)
	{
		MovementComponent->Velocity = FVector::ZeroVector;
	}
	else
	{
		MovementComponent->Velocity -= deltaVelocity;
		MovementComponent->Velocity.Z = 0.0f;
	}
}