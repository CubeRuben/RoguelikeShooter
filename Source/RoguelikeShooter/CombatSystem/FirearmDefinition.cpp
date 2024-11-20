#include "FirearmDefinition.h"

#include "Firearm.h"
#include "FireTypes/FirearmBaseFire.h"
#include "AmmoDefinition.h"

FRandomFloatProperty::FRandomFloatProperty()
{
	MinValue = 0.0f;
	MaxValue = 0.0f;
}

float FRandomFloatProperty::GetRandomValue() const
{
	return FMath::FRandRange(MinValue, MaxValue);
}

FRandomIntProperty::FRandomIntProperty()
{
	MinValue = 0;
	MaxValue = 0;
}

int FRandomIntProperty::GetRandomValue() const
{
	return FMath::RandRange(MinValue, MaxValue);
}

void UFirearmDefinition::GetRequiredAmmoTypes(TSet<UAmmoDefinition*>& RequiredAmmo) const
{
	for (auto& behaviour : OnFireBehaviour)
	{
		if (!behaviour)
			continue;

		RequiredAmmo.Add(behaviour->GetAmmoDefinition());
	}
}

void UFirearmDefinition::OnFire(class UFirearm* Firearm, FVector ShootingDirection)
{
	for (auto& behaviour : OnFireBehaviour) 
	{
		if (!behaviour)
			continue;

		if (Firearm->ConsumeAmmo(behaviour->GetAmmoDefinition(), behaviour->GetAmmoConsumption()))
			behaviour->OnFire(Firearm, ShootingDirection);
	}
}
