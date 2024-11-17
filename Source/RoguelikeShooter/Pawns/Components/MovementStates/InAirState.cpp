#include "InAirState.h"

#include "../PlayerMovementComponent.h"
#include "../../PlayerPawn.h"

SInAirState::SInAirState(UPlayerMovementComponent* NewMovementComponent) : SBaseState(NewMovementComponent) {}

void SInAirState::OnStateExit()
{
	MovementComponent->Velocity.Z = 0.0f;
	MovementComponent->GetPlayerPawn()->GetPlayerInput().bJump = false;
}

void SInAirState::Tick(float DeltaTime)
{
	// Update velocity
	const FVector deltaVerticalVelocity(0.0f, 0.0f, MovementComponent->GetWorld()->GetGravityZ() * DeltaTime);

	MovementComponent->Velocity += deltaVerticalVelocity;

	const FVector input = MovementComponent->GetMovementInput();
	
	if (!input.IsNearlyZero()) 
	{
		const FVector targetVelocity = input * MovementComponent->GetMovementAttributes().MaxWalkingSpeed;
		const FVector projectedVelocity(MovementComponent->Velocity.X, MovementComponent->Velocity.Y, 0.0f);
		const FVector deltaVelocity = (targetVelocity - projectedVelocity) * MovementComponent->GetMovementAttributes().AccelerationInAir * DeltaTime;

		MovementComponent->Velocity += deltaVelocity;
	}

	// Move pawn
	const FVector movementDelta = MovementComponent->Velocity * DeltaTime;

	FHitResult moveHit;
	MovementComponent->SafeMoveUpdatedComponent(movementDelta, MovementComponent->GetPlayerPawn()->GetActorQuat(), true, moveHit);

	if (!moveHit.bBlockingHit)
		return;
	
	if (MovementComponent->CanStayOnSurface(moveHit.ImpactNormal)) 
	{
		MovementComponent->SwitchToState(EMovementState::Idle);
		return;
	}

	FVector normal = moveHit.ImpactNormal;
	normal.Z;
	normal.Normalize();
	MovementComponent->Velocity = FVector::VectorPlaneProject(MovementComponent->Velocity, normal);

	MovementComponent->SlideAlongSurface(movementDelta, 1.0f - moveHit.Time, moveHit.ImpactNormal, moveHit);
}