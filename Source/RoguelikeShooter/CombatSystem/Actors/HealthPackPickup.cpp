#include "HealthPackPickup.h"

#include "../../Pawns/PlayerPawn.h"
#include "../../Pawns/Components/PlayerHealthComponent.h"

AHealthPackPickup::AHealthPackPickup()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	HPToHeal = 50.0f;
}

void AHealthPackPickup::Pickup(APlayerPawn* Pawn)
{
	if (!Pawn)
		return;

	Pawn->GetPlayerHealthComponent()->Heal(HPToHeal);
	Destroy();
}

void AHealthPackPickup::ApplyImpulse(FVector Impulse)
{
	MeshComponent->AddImpulse(Impulse);
}

