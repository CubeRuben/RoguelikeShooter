#include "RocketProjectile.h"

ARocketProjectile::ARocketProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMeshComponent;

	Velocity = FVector::UpVector;
	BaseSpeed = 1500.0f;
	Acceleration = 2000.0f;
}

void ARocketProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Velocity += Velocity.GetSafeNormal() * Acceleration * DeltaTime;
	const FVector deltaMovement = Velocity * DeltaTime;

	FCollisionQueryParams params;
	params.AddIgnoredActor(OwnerActor);
	params.AddIgnoredActor(this);

	FHitResult hit;
	if (!GetWorld()->LineTraceSingleByProfile(hit, GetActorLocation(), GetActorLocation() + deltaMovement, "BlockAll", params))
	{
		AddActorWorldOffset(deltaMovement);
		return;
	}

	Destroy();
}

void ARocketProjectile::InitProjectile(FVector Direction, float Damage, AActor* ShooterActor)
{
	Super::InitProjectile(Direction, Damage, ShooterActor);

	Velocity = Direction * BaseSpeed;

	SetActorRotation(Direction.Rotation());
}
