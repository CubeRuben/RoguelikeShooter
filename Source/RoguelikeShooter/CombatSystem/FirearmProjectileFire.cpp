#include "FirearmProjectileFire.h"

#include "BaseProjectile.h"
#include "Firearm.h"
#include "Damageable.h"
#include "../Pawns/PlayerPawn.h"

UFirearmProjectileFire::UFirearmProjectileFire()
{

}

void UFirearmProjectileFire::OnFire(UFirearm* Firearm, FVector ShootingDirection)
{
	UWorld* world = Firearm->GetWorld();
	
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseProjectile* projectile = world->SpawnActor<ABaseProjectile>(ProjectileClass, params);
	projectile->InitProjectile(ShootingDirection, Firearm->GetDamage(), Firearm->GetOwnerPlayerPawn());
}
