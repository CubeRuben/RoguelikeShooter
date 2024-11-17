#include "FirearmPickup.h"

#include "../Firearm.h"
#include "../FirearmDefinition.h"
#include "../../Pawns/PlayerPawn.h"
#include "../../Pawns/Components/PlayerCombatComponent.h"

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

	if (!Firearm->GetFirearmDefinition())
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

void AFirearmPickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(AFirearmPickup, Firearm))
	{
		InitPickup();
	}
}

