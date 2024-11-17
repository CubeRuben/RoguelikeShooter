#include "PlayerMovementComponent.h"

#include "../PlayerPawn.h"
#include "MovementStates/IdleState.h"
#include "MovementStates/WalkState.h"
#include "MovementStates/RunState.h"
#include "MovementStates/InAirState.h"

#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include <Net/UnrealNetwork.h>

UPlayerMovementComponent::UPlayerMovementComponent()
{
	AddStateToMap(new SIdleState(this));
	AddStateToMap(new SWalkState(this));
	AddStateToMap(new SRunState(this));
	AddStateToMap(new SInAirState(this));
	
	SwitchToState(EMovementState::Idle);

	SetIsReplicated(true);

	PlayerPawn = nullptr;
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerPawn>(GetOwner());

	if (!PlayerPawn)
		DestroyComponent();
}

void UPlayerMovementComponent::BeginDestroy()
{
	Super::BeginDestroy();

	for (auto& pair : StatesMap) 
	{
		if (pair.Value)
			delete pair.Value;
	}
}

void UPlayerMovementComponent::AddStateToMap(SBaseState* NewState)
{
	if (!NewState)
		return;

	StatesMap.Add(NewState->GetStateType(), NewState);
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (PlayerPawn->IsLocallyControlled())
	{
		LocalPlayerTick(DeltaTime);

		ReplicateMovement_ServerRPC(PlayerPawn->GetActorLocation(), PlayerPawn->GetActorRotation().Yaw);
	}
	else 
	{
		ReplicatedPlayerTick(DeltaTime);
	}
}

void UPlayerMovementComponent::LocalPlayerTick(float DeltaTime)
{
	HandleInput(DeltaTime);

	if (CurrentMovementState) 
	{
		CurrentMovementState->Tick(DeltaTime);
	}

	/*

	const FVector startLocation = PlayerPawn->GetActorLocation();

	Velocity.Z += GetWorld()->GetGravityZ() * DeltaTime;

	FHitResult hit;
	SafeMoveUpdatedComponent(Velocity * DeltaTime, PlayerPawn->GetActorQuat(), true, hit);

	if (hit.bBlockingHit)
	{
		const float slopeAngle = FMath::RadiansToDegrees(FMath::Acos(hit.ImpactNormal.Z));

		if (hit.ImpactNormal.Z < 0.0f)
		{
			if (slopeAngle - 90.0f > MovementAttributes.SlopeAngle && Velocity.Z > 0)
			{
				Velocity.Z = 0.0f;
			}
		}
		else
		{
			if (slopeAngle < MovementAttributes.SlopeAngle)
			{
				Velocity.Z = 0.0f;
				bIsInAir = false;
			}
			else
			{
				bIsInAir = true;
			}
		}

		SlideAlongSurface(Velocity * DeltaTime, 1.0f - hit.Time, hit.ImpactNormal, hit);
	}
	else
	{
		bIsInAir = true;
	}

	Velocity = (PlayerPawn->GetActorLocation() - startLocation) / DeltaTime;*/
}

void UPlayerMovementComponent::ReplicatedPlayerTick(float DeltaTime)
{
	const FVector location = PlayerPawn->GetActorLocation();
	const FVector newLocation = FMath::VInterpTo(location, ReplicatedLocation, DeltaTime, 5.0f);

	const FRotator rotation = PlayerPawn->GetActorRotation();
	const FRotator newRotation = FMath::RInterpTo(rotation, FRotator(0.0f, ReplicatedPawnRotation, 0.0f), DeltaTime, 5.0f);

	PlayerPawn->SetActorLocation(newLocation);
	PlayerPawn->SetActorRotation(newRotation);
}

void UPlayerMovementComponent::SwitchToState(EMovementState NewState)
{
	if (CurrentMovementStateType == NewState && CurrentMovementState)
		return;

	if (CurrentMovementState) 
	{
		CurrentMovementState->OnStateExit();
		CurrentMovementState = nullptr;
	}

	SBaseState** state = StatesMap.Find(NewState);

	if (!state)
		return;

	CurrentMovementStateType = NewState;
	CurrentMovementState = *state;
	CurrentMovementState->OnStateEnter();
}

bool UPlayerMovementComponent::CanStayOnSurface(FVector Normal)
{
	const float slopeAngle = FMath::RadiansToDegrees(FMath::Acos(Normal.Z));
	return slopeAngle < MovementAttributes.SlopeAngle;
}

void UPlayerMovementComponent::Jump()
{
	Velocity += FVector(0.0f, 0.0f, MovementAttributes.JumpForce);
}

void UPlayerMovementComponent::SweepGround(float Height, FHitResult& OutHit)
{
	UCapsuleComponent* collider = PlayerPawn->GetCapsuleComponent();

	if (!collider)
		return;

	const float capsuleHeight = collider->GetScaledCapsuleHalfHeight(), capsuleRadius = collider->GetScaledCapsuleRadius();
	const float heightScale = 0.95f;
	const float heightDelta = capsuleHeight * (1.f - heightScale);
	FCollisionShape customCollisionShape = FCollisionShape::MakeCapsule(capsuleRadius, capsuleHeight - heightDelta);

	const FVector componentLocation = collider->GetComponentLocation();
	const FVector endLocation = componentLocation + FVector(0.f, 0.f, -Height - heightDelta);

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(PlayerPawn);

	GetWorld()->SweepSingleByProfile(OutHit, componentLocation, endLocation, FQuat::Identity, collider->GetCollisionProfileName(), customCollisionShape, collisionParams);
}

void UPlayerMovementComponent::HandleInput(float DeltaTime)
{
	FPlayerInput& playerInput = PlayerPawn->GetPlayerInput();

	const float sensitivity = 1.5f;

	UCameraComponent* playerCamera = PlayerPawn->GetCameraComponent();
	const float newAngle = playerCamera->GetRelativeRotation().Pitch + playerInput.MouseY * sensitivity;
	const float clampedAngle = FMath::Clamp(newAngle, -85.0f, 85.0f);
	playerCamera->SetRelativeRotation(FRotator(clampedAngle, 0.0f, 0.0f));

	PlayerPawn->AddActorLocalRotation(FRotator(0.0f, playerInput.MouseX * sensitivity, 0.0f));
}

FVector UPlayerMovementComponent::GetMovementInput()
{
	FPlayerInput& playerInput = PlayerPawn->GetPlayerInput();
	FVector movementInput(0.0f);

	if (playerInput.bMoveForward)
		movementInput += PlayerPawn->GetActorForwardVector();
		
	if (playerInput.bMoveBack)
		movementInput -= PlayerPawn->GetActorForwardVector();

	if (playerInput.bMoveRight)
		movementInput += PlayerPawn->GetActorRightVector();

	if (playerInput.bMoveLeft)
		movementInput -= PlayerPawn->GetActorRightVector();
	
	return movementInput.GetSafeNormal2D();
}

void UPlayerMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerMovementComponent, ReplicatedLocation);
	DOREPLIFETIME(UPlayerMovementComponent, ReplicatedPawnRotation);
}

void UPlayerMovementComponent::ReplicateMovement_ServerRPC_Implementation(FVector Location, float PawnRotation)
{
	ReplicatedLocation = Location;
	ReplicatedPawnRotation = PawnRotation;
}


