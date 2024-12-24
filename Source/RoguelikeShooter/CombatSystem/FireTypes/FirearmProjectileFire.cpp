#include "FirearmProjectileFire.h"

#include "../Projectiles/BaseProjectile.h"
#include "../Firearm.h"
#include "../../Pawns/PlayerPawn.h"

#include <Camera/CameraComponent.h>

UFirearmProjectileFire::UFirearmProjectileFire()
{

}

void UFirearmProjectileFire::OnFire(UFirearm* Firearm, FVector ShootingDirection)
{
	if (!Firearm->GetOwnerPlayerPawn()->HasAuthority() && !Firearm->GetOwnerPlayerPawn()->IsLocallyControlled())
		return;

	UWorld* world = Firearm->GetWorld();
	
	const FVector startPosition = Firearm->GetOwnerPlayerPawn()->GetClientCameraLocation() + Firearm->GetShootingOffset();
	
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseProjectile* projectile = world->SpawnActor<ABaseProjectile>(ProjectileClass, startPosition, FRotator::ZeroRotator, params);
	projectile->InitProjectile(ShootingDirection, Firearm->GetDamage(), Firearm->GetOwnerPlayerPawn());
}
