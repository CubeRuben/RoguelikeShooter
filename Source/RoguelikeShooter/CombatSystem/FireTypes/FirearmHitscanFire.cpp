#include "FirearmHitscanFire.h"

#include "../Firearm.h"
#include "../Damageable.h"
#include "../../Pawns/PlayerPawn.h"

#include <Camera/CameraComponent.h>
#include <NiagaraFunctionLibrary.h>
#include <NiagaraComponent.h>
#include <NiagaraSystem.h>


UFirearmHitscanFire::UFirearmHitscanFire()
{
	HitMaxDistance = 5000.0f;
	ScatterAngle = 1.0f;
}

void UFirearmHitscanFire::OnFire(UFirearm* Firearm, FVector ShootingDirection)
{
	APlayerPawn* playerPawn = Firearm->GetOwnerPlayerPawn();
	const FVector startPosition = playerPawn->GetClientCameraLocation() + Firearm->GetShootingOffset();

	const float xRotationOffset = FMath::FRandRange(-ScatterAngle, ScatterAngle);
	const float yRotationOffset = FMath::FRandRange(-ScatterAngle, ScatterAngle);
	const float zRotationOffset = FMath::FRandRange(-ScatterAngle, ScatterAngle);

	const FVector rotatedShootingDirection = ShootingDirection
		.RotateAngleAxis(xRotationOffset, FVector::ForwardVector)
		.RotateAngleAxis(yRotationOffset, FVector::RightVector)
		.RotateAngleAxis(zRotationOffset, FVector::UpVector);

	const FVector endPosition = startPosition + rotatedShootingDirection * HitMaxDistance;

	FCollisionQueryParams params;
	params.AddIgnoredActor(playerPawn);

	FHitResult hitResult;
	if (!Firearm->GetWorld()->LineTraceSingleByProfile(hitResult, startPosition, endPosition, "BlockAll", params)) 
	{
		SpawnVisual(Firearm, startPosition, endPosition);
		return;
	}

	SpawnVisual(Firearm, startPosition, hitResult.Location);

	if (!playerPawn->HasAuthority())
		return;

	if (!hitResult.GetActor())
		return;

	IDamageable* damageable = Cast<IDamageable>(hitResult.GetActor());

	if (!damageable)
		return;

	FDamageParams damageParams;
	damageParams.DamageSource = playerPawn;
	damageParams.HitDirection = ShootingDirection;
	damageParams.HitLocation = hitResult.Location;

	damageable->ApplyDamage(Firearm->GetDamage(), &damageParams);
}

void UFirearmHitscanFire::SpawnVisual(UFirearm* Firearm, FVector StartLocation, FVector EndLocation)
{
	if (!TraceParticleSystem)
		return;

	UNiagaraComponent* niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(Firearm->GetOwnerPlayerPawn(), TraceParticleSystem, StartLocation);

	if (!niagaraComponent)
		return;

	niagaraComponent->SetVectorParameter(TEXT("TraceEnd"), EndLocation - StartLocation);
}
