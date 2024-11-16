#include "FirearmDefinition.h"

#include "Firearm.h"
#include "FirearmBaseFire.h"

FRandomFloatProperty::FRandomFloatProperty()
{
	MinValue = 0.0f;
	MaxValue = 0.0f;
}

float FRandomFloatProperty::GetRandomValue() const
{
	return FMath::FRandRange(MinValue, MaxValue);
}

void UFirearmDefinition::OnFire(class UFirearm* Firearm)
{
	for (auto& behaviour : OnFireBehaviour) 
	{
		if (!behaviour)
			continue;

		behaviour->OnFire(Firearm);
	}
}
