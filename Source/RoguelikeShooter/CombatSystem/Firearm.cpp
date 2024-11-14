#include "Firearm.h"

#include "FirearmDefinition.h"
#include "../Pawns/PlayerPawn.h"

UFirearm::UFirearm()
{
	bIsAutoFireMode = false;
}

void UFirearm::InitFirearm()
{
	if (!FirearmDefinition)
		return;

	Damage = FirearmDefinition->GetDamage();
	FireRate = FirearmDefinition->GetFireRate();
}

void UFirearm::SetFirearmDefinition(UFirearmDefinition* NewFirearmDefinition)
{
	FirearmDefinition = NewFirearmDefinition;
	
	InitFirearm();
}

bool UFirearm::Fire()
{
	FirearmDefinition->OnFire(this);

	return true;
}

void UFirearm::AlternativeAction() 
{

}

void UFirearm::SetOwnerPlayerPawn(APlayerPawn* NewOwnerPlayerPawn)
{
	OwnerPlayerPawn = NewOwnerPlayerPawn;
}

APlayerPawn* UFirearm::GetOwnerPlayerPawn() const
{
	return OwnerPlayerPawn.Get();
}