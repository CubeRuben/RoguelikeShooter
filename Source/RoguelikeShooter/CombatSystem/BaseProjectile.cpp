#include "BaseProjectile.h"

ABaseProjectile::ABaseProjectile()
{
	bReplicates = true;
}

void ABaseProjectile::InitProjectile(FVector Direction, float Damage, AActor* ShooterActor)
{
	BaseDamage = Damage;
	OwnerActor = ShooterActor;
}

