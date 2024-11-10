#include "BaseState.h"

#include "../PlayerMovementComponent.h"
#include "../../PlayerPawn.h"

SBaseState::SBaseState(UPlayerMovementComponent* NewMovementComponent)
{
	MovementComponent = NewMovementComponent;
}

SIdleState::SIdleState(UPlayerMovementComponent* NewMovementComponent) : SBaseState(NewMovementComponent) {}

void SIdleState::OnStateEnter()
{
	MovementComponent->Velocity.Z = 0.0f;
}

void SIdleState::Tick(float DeltaTime)
{
	FPlayerInput& playerInput = MovementComponent->GetPlayerPawn()->GetPlayerInput();

	// Handle input
	if (playerInput.bJump) 
	{
		MovementComponent->SwitchToState(EMovementState::InAir);
		return;
	}

	if (playerInput.bMoveForward || playerInput.bMoveBack || playerInput.bMoveRight || playerInput.bMoveLeft) 
	{
		//if (playerInput.bRun && playerInput.bMoveForward && !playerInput.bMoveBack && !playerInput.bMoveRight && !playerInput.bMoveLeft)
		//{
		//	MovementComponent->SwitchToState(EMovementState::Run);
		//}
		//else 
		{
			MovementComponent->SwitchToState(EMovementState::Walk);
		}
		return;
	}

	// Update velocity
	const FVector deltaVelocity = MovementComponent->Velocity * MovementComponent->GetMovementAttributes().StoppingAcceleration * DeltaTime;
	
	if (deltaVelocity.SizeSquared() < 1.0f)
	{
		MovementComponent->Velocity = FVector::ZeroVector;
	}
	else 
	{
		MovementComponent->Velocity -= deltaVelocity;
		MovementComponent->Velocity.Z = 0.0f;
	}

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

SWalkState::SWalkState(UPlayerMovementComponent* NewMovementComponent) : SBaseState(NewMovementComponent) {}

void SWalkState::Tick(float DeltaTime)
{
	FPlayerInput& playerInput = MovementComponent->GetPlayerPawn()->GetPlayerInput();

	// Handle input
	if (playerInput.bJump)
	{
		MovementComponent->SwitchToState(EMovementState::InAir);
		return;
	}

	if (!(playerInput.bMoveForward || playerInput.bMoveBack || playerInput.bMoveRight || playerInput.bMoveLeft))
	{
		MovementComponent->SwitchToState(EMovementState::Idle);
		return;
	}

	//if (playerInput.bRun && playerInput.bMoveForward && !playerInput.bMoveBack && !playerInput.bMoveRight && !playerInput.bMoveLeft)
	//{
	//	MovementComponent->SwitchToState(EMovementState::Run);
	//	return;
	//}

	// Update velocity
	const FVector targetVelocity = MovementComponent->GetMovementInput() * MovementComponent->GetMovementAttributes().MaxWalkingSpeed;
	const FVector projectedVelocity(MovementComponent->Velocity.X, MovementComponent->Velocity.Y, 0.0f);
	const FVector deltaVelocity = (targetVelocity - projectedVelocity) * MovementComponent->GetMovementAttributes().Acceleration * DeltaTime;

	MovementComponent->Velocity += deltaVelocity;

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

SInAirState::SInAirState(UPlayerMovementComponent* NewMovementComponent) : SBaseState(NewMovementComponent) {}

void SInAirState::OnStateExit()
{
	MovementComponent->Velocity.Z = 0.0f;
}

void SInAirState::Tick(float DeltaTime)
{
	// Update velocity
	const FVector deltaVelocity(0.0f, 0.0f, MovementComponent->GetWorld()->GetGravityZ() * DeltaTime);

	MovementComponent->Velocity += deltaVelocity;

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

	MovementComponent->SlideAlongSurface(movementDelta, 1.0f - moveHit.Time, moveHit.ImpactNormal, moveHit);
}
