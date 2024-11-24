#include "RocketProjectile.h"

#include "../Actors/Explosion.h"

#include <Net/UnrealNetwork.h>

ARocketProjectile::ARocketProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMeshComponent;

	Velocity = FVector::UpVector;
	BaseSpeed = 1500.0f;
	Acceleration = 2000.0f;
	BaseExplosionImpulse = 2000.0f;

	Location_Replicated = FVector::ZeroVector;
}

void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();

	Location_Replicated = GetActorLocation();
}

void ARocketProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARocketProjectile, Location_Replicated);
}

void ARocketProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) 
	{
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), Location_Replicated, DeltaTime, 10.0f));
		return;
	}

	Velocity += Velocity.GetSafeNormal() * Acceleration * DeltaTime;
	const FVector deltaMovement = Velocity * DeltaTime;

	FCollisionQueryParams params;
	params.AddIgnoredActor(OwnerActor);
	params.AddIgnoredActor(this);

	FHitResult hit;
	if (!GetWorld()->LineTraceSingleByProfile(hit, GetActorLocation(), GetActorLocation() + deltaMovement, "BlockAll", params))
	{
		AddActorWorldOffset(deltaMovement);
		Location_Replicated = GetActorLocation();
		return;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AExplosion* explosion = GetWorld()->SpawnActor<AExplosion>(AExplosion::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, spawnParams);
	
	if (explosion)
		explosion->InitExplosion(this, BaseDamage, 7.0f * 100.0f, BaseExplosionImpulse);

	Destroy();
}

void ARocketProjectile::InitProjectile(FVector Direction, float Damage, AActor* ShooterActor)
{
	Super::InitProjectile(Direction, Damage, ShooterActor);

	Velocity = Direction * BaseSpeed;

	SetActorRotation(Direction.Rotation());
}
