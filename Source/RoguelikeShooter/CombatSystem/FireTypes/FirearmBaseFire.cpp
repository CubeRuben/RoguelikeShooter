#include "FirearmBaseFire.h"

#include "../Firearm.h"

UFirearmBaseFire::UFirearmBaseFire()
{
	AmmoDefinition = nullptr;
	AmmoConsumption = 1;
}

void UFirearmBaseFire::OnFire(UFirearm* Firearm, FVector ShootingDirection)
{
	
}
