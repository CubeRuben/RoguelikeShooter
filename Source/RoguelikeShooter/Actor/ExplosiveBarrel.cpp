#include "ExplosiveBarrel.h"

#include "../CombatSystem/Actors/Explosion.h"

AExplosiveBarrel::AExplosiveBarrel()
{
	bReplicates = true;
	SetReplicateMovement(true);

	MinTimeForExplosion = 0.5f;
	MaxTimeForExplosion = 5.0f;
}

void AExplosiveBarrel::OnDestroy()
{
	if (ExplosionTimerHandle.IsValid())
		return;

	const float timer = FMath::RandRange(MinTimeForExplosion, MaxTimeForExplosion);
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AExplosiveBarrel::Explode, timer);

	OnSetOnFire();
}

void AExplosiveBarrel::Explode()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AExplosion* explosion = GetWorld()->SpawnActor<AExplosion>(AExplosion::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, spawnParams);

	if (explosion)
		explosion->InitExplosion(this, 125.0f, 10.0f * 100.0f, 500000.0f);

	Destroy();
}