#include "Firearm.h"

#include "FirearmDefinition.h"
#include "AmmoDefinition.h"
#include "../Pawns/PlayerPawn.h"
#include "../Pawns/Components/PlayerCombatComponent.h"

#include <Camera/CameraComponent.h>

UFirearm::UFirearm()
{
	Damage = 0.0f;
	FireRate = 0.0f;

	AmmoCapacity = 0;
	AmmoReloadTime = 0.0f;

	bIsAutoFireMode = false;
}

void UFirearm::InitFirearm()
{
	if (!FirearmDefinition)
		return;

	Damage = FirearmDefinition->GetDamage();
	FireRate = FirearmDefinition->GetFireRate();
	AmmoCapacity = FirearmDefinition->GetAmmoCapacity();
	AmmoReloadTime = FirearmDefinition->GetAmmoReloadTime();

	TSet<UAmmoDefinition*> requiredAmmoTypes;
	FirearmDefinition->GetRequiredAmmoTypes(requiredAmmoTypes);

	for (auto& ammoDefinition : requiredAmmoTypes) 
	{
		ContainedAmmo.Add(ammoDefinition, AmmoCapacity);
	}
}

void UFirearm::SetFirearmDefinition(UFirearmDefinition* NewFirearmDefinition)
{
	FirearmDefinition = NewFirearmDefinition;
	
	InitFirearm();
}

FVector UFirearm::GetShootingOffset() const
{
	if (!OwnerPlayerPawn.IsValid())
		return FirearmDefinition->GetShootingOffset();

	return OwnerPlayerPawn->GetCameraComponent()->GetComponentRotation().RotateVector(FirearmDefinition->GetShootingOffset());
}

bool UFirearm::Fire()
{
	if (NextTimeToFire > GetWorld()->GetTimeSeconds())
		return false;

	const FVector shootingDirection = OwnerPlayerPawn->GetPlayerCombatComponent()->GetAimAdjustDirection(GetShootingOffset());
	FirearmDefinition->OnFire(this, shootingDirection);

	if (FireRate == 0)
		return true;

	NextTimeToFire = GetWorld()->GetTimeSeconds() + 1.0f / FireRate;
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

bool UFirearm::CanConsumeAmmo(UAmmoDefinition* AmmoDefinition, int AmmoAmount)
{
	int* ammoAmount = ContainedAmmo.Find(AmmoDefinition);

	if (!ammoAmount)
		return false;

	if (*ammoAmount < AmmoAmount)
		return false;

	return true;
}

bool UFirearm::ConsumeAmmo(UAmmoDefinition* AmmoDefinition, int AmmoAmount)
{
	if (CanConsumeAmmo(AmmoDefinition, AmmoAmount))
	{
		*ContainedAmmo.Find(AmmoDefinition) -= AmmoAmount;
		return true;
	}

	return false;
}

bool UFirearm::CanReloadAmmo()
{
	for (auto& ammo : ContainedAmmo) 
	{
		const bool notFull = ammo.Value < AmmoCapacity;
		const bool haveFreeAmmo = OwnerPlayerPawn->GetPlayerCombatComponent()->GetAmmoAmount(ammo.Key) > 0;
		if (notFull && haveFreeAmmo)
			return true;
	}

	return false;
}

void UFirearm::ReloadAmmo()
{
	for (auto& ammo : ContainedAmmo) 
	{
		const int availableAmmo = OwnerPlayerPawn->GetPlayerCombatComponent()->GetAmmoAmount(ammo.Key);
		const int needAmmo = AmmoCapacity - ammo.Value;
		const int ammoToConsume = FMath::Min(availableAmmo, needAmmo);

		if (!OwnerPlayerPawn->GetPlayerCombatComponent()->ConsumeAmmo(ammo.Key, ammoToConsume))
			continue;
		
		ammo.Value += ammoToConsume;

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Consumed ammo");
	}
}

void UFirearm::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(UFirearm, FirearmDefinition))
	{
		InitFirearm();
	}
}