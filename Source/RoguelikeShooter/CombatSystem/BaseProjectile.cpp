#include "BaseProjectile.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

void ABaseProjectile::InitProjectile(FVector Direction, float Damage, AActor* OwnerActor)
{

}

