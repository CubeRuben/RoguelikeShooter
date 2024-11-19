#include "DamageableProp.h"

ADamageableProp::ADamageableProp()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetSimulatePhysics(true);
	RootComponent = StaticMeshComponent;

	HealthPoints = 100.0f;
}

void ADamageableProp::OnDestroy()
{
	Destroy();
}

void ADamageableProp::ApplyDamage(float DamageAmount, FDamageParams* DamageParams)
{
	HealthPoints -= DamageAmount;

	if (HealthPoints <= 0.0f)
	{
		OnDestroy();
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


