#include "FirearmProjectileFire.h"

#include "BaseProjectile.h"
#include "Firearm.h"
#include "../Pawns/PlayerPawn.h"

#include <Camera/CameraComponent.h>

UFirearmProjectileFire::UFirearmProjectileFire()
{

}

void UFirearmProjectileFire::OnFire(UFirearm* Firearm, FVector ShootingDirection)
{
	UWorld* world = Firearm->GetWorld();
	
	const FVector startPosition = Firearm->GetOwnerPlayerPawn()->GetCameraComponent()->GetComponentLocation() + Firearm->GetShootingOffset();
	
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseProjectile* projectile = world->SpawnActor<ABaseProjectile>(ProjectileClass, startPosition, FRotator::ZeroRotator, params);
	projectile->InitProjectile(ShootingDirection, Firearm->GetDamage(), Firearm->GetOwnerPlayerPawn());
}
