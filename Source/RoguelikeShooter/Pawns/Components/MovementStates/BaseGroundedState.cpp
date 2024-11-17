#include "BaseGroundedState.h"

#include "../PlayerMovementComponent.h"
#include "../../PlayerPawn.h"

SBaseGroundedState::SBaseGroundedState(UPlayerMovementComponent* NewMovementComponent) : SBaseState(NewMovementComponent) {}

void SBaseGroundedState::OnStateEnter()
{
	MovementComponent->Velocity.Z = 0.0f;
}

void SBaseGroundedState::Tick(float DeltaTime)
{
	if (MovementComponent->Velocity.Z > 0)
	{
		MovementComponent->SwitchToState(EMovementState::InAir);
		return;
	}

	if (UpdateInput())
		return;

	UpdateVelocity(DeltaTime);

	// Move pawn
	const FVector movementDelta = MovementComponent->Velocity * DeltaTime;

	FHitResult moveHit;
	MovementComponent->SafeMoveUpdatedComponent(movementDelta, MovementComponent->GetPlayerPawn()->GetActorQuat(), true, moveHit);

	if (moveHit.bBlockingHit)
	{
		MovementComponent->SlideAlongSurface(movementDelta, 1.0f - moveHit.Time, moveHit.ImpactNormal, moveHit);
	}

	// Check ground
	const float groundCheckDistance = 25.0f;
	FHitResult sweepHit;
	MovementComponent->SweepGround(groundCheckDistance, sweepHit);

	if (!sweepHit.bBlockingHit || !MovementComponent->CanStayOnSurface(sweepHit.ImpactNormal))
	{
		MovementComponent->SwitchToState(EMovementState::InAir);
		return;
	}

	MovementComponent->SafeMoveUpdatedComponent(FVector(0.0f, 0.0f, -groundCheckDistance), MovementComponent->GetPlayerPawn()->GetActorQuat(), true, moveHit);
}