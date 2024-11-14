#include "FirearmPickup.h"

#include "Firearm.h"
#include "FirearmDefinition.h"
#include "../Pawns/PlayerPawn.h"
#include "../Pawns/Components/PlayerCombatComponent.h"

AFirearmPickup::AFirearmPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	FirearmMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Firearm Mesh Component"));
	FirearmMeshComponent->SetSimulatePhysics(true);
	RootComponent = FirearmMeshComponent;
}

void AFirearmPickup::BeginPlay()
{
	Super::BeginPlay();
	
	InitPickup();
}

void AFirearmPickup::InitPickup()
{
	if (!Firearm)
		return;

	FirearmMeshComponent->SetStaticMesh(Firearm->GetFirearmDefinition()->GetFirearmMesh());
}

void AFirearmPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFirearmPickup::Interact(APlayerPawn* Pawn)
{
	if (!Pawn)
		return;

	Pawn->GetPlayerCombatComponent()->AddFirearm(Firearm);
	Destroy();
}

