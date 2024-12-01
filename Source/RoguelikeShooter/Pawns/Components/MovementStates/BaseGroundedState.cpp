#include "BaseGroundedState.h"

#include "../PlayerMovementComponent.h"
#include "../../PlayerPawn.h"

SBaseGroundedState::SBaseGroundedState(UPlayerMovementComponent* NewMovementComponent) : SBaseState(NewMovementComponent) {}

bool SBaseGroundedState::StepUp(const FVector& MovementDelta)
{
	static const float MaxStepUpHeight = 20;
	static const float SpaceForStepUp = 10;

	float availableStepUpHeight = MaxStepUpHeight;

	FHitResult ceilingHit;

	FVector moveDelta = MovementDelta * FVector(1, 1, 0);

	MovementComponent->SweepMovementCollider(FVector::ZeroVector, FVector(0, 0, availableStepUpHeight), ceilingHit);

	if (ceilingHit.bBlockingHit)
	{
		availableStepUpHeight = ceilingHit.Distance;
	}

	FHitResult wallHit;

	MovementComponent->SweepMovementCollider(FVector(0, 0, availableStepUpHeight), FVector(0, 0, availableStepUpHeight) + moveDelta, wallHit);

	if (wallHit.bBlockingHit)
	{
		if (wallHit.Distance <= SpaceForStepUp)
		{
			return false;
		}
	}

	FHitResult floorHit;

	MovementComponent->SweepMovementCollider(FVector(0, 0, availableStepUpHeight) + moveDelta * wallHit.Time, moveDelta * wallHit.Time, floorHit);

	if (floorHit.bBlockingHit)
	{
		if (!MovementComponent->CanStayOnSurface(floorHit.ImpactNormal))
		{
			return false;
		}

		MovementComponent->GetPlayerPawn()->SetActorLocation(floorHit.Location + FVector(0, 0, 1));
		return true;
	}

	return false;
}

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
		if (!StepUp(movementDelta))
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