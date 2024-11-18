#include "FirearmDefinition.h"

#include "Firearm.h"
#include "FireTypes/FirearmBaseFire.h"
#include "../Pawns/Components/PlayerCombatComponent.h"
#include "../Pawns/PlayerPawn.h"
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

void UFirearmDefinition::OnFire(class UFirearm* Firearm, FVector ShootingDirection)
{
	for (auto& behaviour : OnFireBehaviour) 
	{
		if (!behaviour)
			continue;

		if (Firearm->GetOwnerPlayerPawn()->GetPlayerCombatComponent()->ConsumeAmmo(behaviour->GetAmmoDefinition(), behaviour->GetAmmoConsumption()))
			behaviour->OnFire(Firearm, ShootingDirection);
	}
}
