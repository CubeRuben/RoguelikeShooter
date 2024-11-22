#include "AmmoPickup.h"

#include "../../Pawns/PlayerPawn.h"
#include "../../Pawns/Components/PlayerCombatComponent.h"

AAmmoPickup::AAmmoPickup()
{
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionProfileName("Pickup");
	RootComponent = MeshComponent;

	AmmoDefinition = nullptr;
	AmmoAmount = 30;
}

void AAmmoPickup::Pickup(APlayerPawn* Pawn)
{
	if (!Pawn)
		return;

	if (Pawn->GetPlayerCombatComponent()->AddAmmo(AmmoDefinition, AmmoAmount))
		Destroy();
}

void AAmmoPickup::ApplyImpulse(FVector Impulse)
{
	MeshComponent->AddImpulse(Impulse);
}


