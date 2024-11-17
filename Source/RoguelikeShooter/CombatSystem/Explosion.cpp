#include "Explosion.h"

#include "EngineUtils.h"
#include "Damageable.h"

AExplosion::AExplosion()
{
	//PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
}

void AExplosion::InitExplosion(AActor* ExplosionOwner, float Damage, float DamageDistance, float Impulse)
{
	const float damageDistanceSquared = DamageDistance * DamageDistance;

	for (TActorIterator<AActor> iterator(GetWorld()); iterator; ++iterator) 
	{
		AActor* actor = *iterator;

		if (!actor) 
			continue;

		IDamageable* damageable = Cast<IDamageable>(actor);

		if (!damageable)
			continue;

		const float distanceToTargetSquared = FVector::DistSquared(GetActorLocation(), actor->GetActorLocation());

		if (distanceToTargetSquared > damageDistanceSquared)
			continue;

		FCollisionQueryParams collisionParams;
		collisionParams.AddIgnoredActor(this);
		collisionParams.AddIgnoredActor(actor);
		collisionParams.AddIgnoredActor(ExplosionOwner);

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByProfile(hit, GetActorLocation(), actor->GetActorLocation(), "BlockAll", collisionParams))
			continue;

		FDamageParams params;
		params.DamageSource = this;

		const float ratio = 1.0f - distanceToTargetSquared / damageDistanceSquared;

		damageable->ApplyDamage(Damage * ratio, &params);
		
		const FVector impulseDirection = (actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		damageable->ApplyImpulse(impulseDirection * Impulse * ratio);
	}
}
