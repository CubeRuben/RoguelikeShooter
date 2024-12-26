#include "Explosion.h"

#include "EngineUtils.h"
#include "../Damageable.h"

#include <NiagaraFunctionLibrary.h>
#include <NiagaraComponent.h>
#include <NiagaraSystem.h>
#include <Sound/SoundCue.h>
#include <Kismet/GameplayStatics.h>

AExplosion::AExplosion()
{
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> ExplosionNiagaraSystemAsset(TEXT("/Game/VFX/Explosion/NS_Explosion.NS_Explosion"));

	if (ExplosionNiagaraSystemAsset.Object)
	{
		NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
		NiagaraComponent->SetupAttachment(RootComponent);
		NiagaraComponent->SetAsset(ExplosionNiagaraSystemAsset.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundCue> ExplosionSoundAsset(TEXT("/Game/SFX/Explosion/SC_Explosion.SC_Explosion"));

	if (ExplosionSoundAsset.Object) 
	{
		ExplosionSound = ExplosionSoundAsset.Object;
	}
}

void AExplosion::BeginPlay()
{
	Super::BeginPlay();

	if (!NiagaraComponent)
		return;

	NiagaraComponent->OnSystemFinished.AddDynamic(this, &AExplosion::OnSystemFinished);
	NiagaraComponent->Activate(true);

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
}

void AExplosion::OnSystemFinished(UNiagaraComponent* System)
{
	Destroy();
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

		const float distanceToTargetSquared = FVector::DistSquared(GetActorLocation(), damageable->GetLocation());

		if (distanceToTargetSquared > damageDistanceSquared)
			continue;

		FCollisionQueryParams collisionParams;
		collisionParams.AddIgnoredActor(this);
		collisionParams.AddIgnoredActor(actor);
		collisionParams.AddIgnoredActor(ExplosionOwner);

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByProfile(hit, GetActorLocation(), damageable->GetLocation(), "BlockAll", collisionParams))
			continue;

		FDamageParams params;
		params.DamageSource = this;

		const float ratio = 1.0f - distanceToTargetSquared / damageDistanceSquared;

		damageable->ApplyDamage(Damage * ratio, &params);
		
		const FVector impulseDirection = (damageable->GetLocation() - GetActorLocation()).GetSafeNormal();
		damageable->ApplyImpulse(impulseDirection * Impulse * ratio);
	}
}
