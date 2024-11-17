#include "DamageableProp.h"

ADamageableProp::ADamageableProp()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetSimulatePhysics(true);

	HealthPoints = 100.0f;
}

void ADamageableProp::ApplyDamage(float DamageAmount, FDamageParams* DamageParams)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Damaged");

	HealthPoints -= DamageAmount;

	if (HealthPoints <= 0.0f)
	{
		Destroy();
		return;
	}

	if (!DamageParams)
		return;

	if (StaticMeshComponent->IsSimulatingPhysics())
		StaticMeshComponent->AddForceAtLocation(DamageParams->HitDirection * DamageAmount * 10.f, DamageParams->HitLocation);
}

void ADamageableProp::ApplyImpulse(FVector Impulse)
{
	StaticMeshComponent->AddImpulse(Impulse);
}


