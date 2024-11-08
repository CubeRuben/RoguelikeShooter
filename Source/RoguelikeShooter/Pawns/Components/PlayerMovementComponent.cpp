#include "PlayerMovementComponent.h"

#include "../PlayerPawn.h"

#include <Camera/CameraComponent.h>
#include <Net/UnrealNetwork.h>

UPlayerMovementComponent::UPlayerMovementComponent()
{
	bIsInAir = false;
	
	SetIsReplicated(true);
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerPawn>(GetOwner());

	if (!PlayerPawn)
		DestroyComponent();
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

	Velocity = (PlayerPawn->GetActorLocation() - startLocation) / DeltaTime;
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

void UPlayerMovementComponent::HandleInput(float DeltaTime)
{
	FPlayerInput& playerInput = PlayerPawn->GetPlayerInput();

	const float sensitivity = 1.5f;

	UCameraComponent* playerCamera = PlayerPawn->GetCameraComponent();
	const float newAngle = playerCamera->GetRelativeRotation().Pitch + playerInput.MouseY * sensitivity;
	const float clampedAngle = FMath::Clamp(newAngle, -85.0f, 85.0f);
	playerCamera->SetRelativeRotation(FRotator(clampedAngle, 0.0f, 0.0f));

	PlayerPawn->AddActorLocalRotation(FRotator(0.0f, playerInput.MouseX * sensitivity, 0.0f));

	const FVector targetVelocity = GetMovementInput() * GetTargetSpeed();
	const FVector projectedVelocity(Velocity.X, Velocity.Y, 0.0f);
	const FVector deltaVelocity = (targetVelocity - projectedVelocity) * MovementAttributes.Acceleration * DeltaTime;
	
	Velocity += deltaVelocity;

	if (playerInput.bJump)
	{
		playerInput.bJump = false;

		if (!bIsInAir)
			Velocity += FVector(0.0f, 0.0f, MovementAttributes.JumpForce);
	}
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

float UPlayerMovementComponent::GetTargetSpeed()
{
	FPlayerInput& playerInput = PlayerPawn->GetPlayerInput();

	if (!(playerInput.bMoveForward || playerInput.bMoveBack || playerInput.bMoveLeft || playerInput.bMoveRight))
		return 0.0f;
	
	if (playerInput.bRun)
		return MovementAttributes.MaxRunningSpeed;

	return MovementAttributes.MaxWalkingSpeed;
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


