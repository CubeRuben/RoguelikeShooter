#include "FirearmHitscanFire.h"

#include "Firearm.h"
#include "Damageable.h"
#include "../Pawns/PlayerPawn.h"

#include <Camera/CameraComponent.h>

UFirearmHitscanFire::UFirearmHitscanFire()
{
	HitMaxDistance = 5000.0f;
	ScatterAngle = 1.0f;
}

void UFirearmHitscanFire::OnFire(UFirearm* Firearm, FVector ShootingDirection)
{
	APlayerPawn* playerPawn = Firearm->GetOwnerPlayerPawn();
	const UCameraComponent* cameraComponent = playerPawn->GetCameraComponent();
	const FVector startPosition = cameraComponent->GetComponentLocation() + Firearm->GetShootingOffset();
	const FVector endPosition = startPosition + ShootingDirection * HitMaxDistance;

	FCollisionQueryParams params;
	params.AddIgnoredActor(playerPawn);

	FHitResult hitResult;
	if (!Firearm->GetWorld()->LineTraceSingleByProfile(hitResult, startPosition, endPosition, "BlockAll", params))
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
