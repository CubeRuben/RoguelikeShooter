#include "BombProjectile.h"

#include "Damageable.h"
#include "Explosion.h"

ABombProjectile::ABombProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->BodyInstance.bNotifyRigidBodyCollision = true;
	RootComponent = StaticMeshComponent;

	BaseExplosionImpulse = 50000.0f;
	BaseSpeed = 2500.0f;
	Lifetime = 3.0f;
	bEverHitSurface = false;
}

void ABombProjectile::BeginPlay()
{
	Super::BeginPlay();

	DetonationTime = GetWorld()->GetTimeSeconds() + Lifetime;

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ABombProjectile::OnComponentHit);
}

void ABombProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bEverHitSurface)
		return;

	if (!OtherActor)
		return;

	IDamageable* damageable = Cast<IDamageable>(OtherActor);

	if (!damageable)
	{
		bEverHitSurface = true;
		return;
	}

	SpawnExplosion();
}

void ABombProjectile::SpawnExplosion()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AExplosion* explosion = GetWorld()->SpawnActor<AExplosion>(AExplosion::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, spawnParams);

	if (explosion)
		explosion->InitExplosion(this, BaseDamage, 7.0f * 100.0f, BaseExplosionImpulse);

	Destroy();
}

void ABombProjectile::InitProjectile(FVector Direction, float Damage, AActor* ShooterActor)
{
	Super::InitProjectile(Direction, Damage, ShooterActor);

	StaticMeshComponent->SetPhysicsLinearVelocity(Direction * BaseSpeed);
}

void ABombProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DetonationTime < GetWorld()->GetTimeSeconds()) 
	{
		SpawnExplosion();
	}
}
