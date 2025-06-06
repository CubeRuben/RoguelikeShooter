#include "Firearm.h"

#include "FirearmDefinition.h"
#include "AmmoDefinition.h"
#include "../Pawns/PlayerPawn.h"
#include "../Pawns/Components/PlayerCombatComponent.h"

#include <Camera/CameraComponent.h>
#include <Net/UnrealNetwork.h>
#include <Kismet/GameplayStatics.h>

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

	ContainedAmmo.Empty();
	for (auto& ammoDefinition : requiredAmmoTypes) 
	{
		ContainedAmmo.Add(FAmmoContainer(ammoDefinition, AmmoCapacity));
	}
}

void UFirearm::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFirearm, OwnerPlayerPawn);
	DOREPLIFETIME(UFirearm, FirearmDefinition);
	DOREPLIFETIME(UFirearm, Damage);
	DOREPLIFETIME(UFirearm, FireRate);
	DOREPLIFETIME(UFirearm, AmmoCapacity);
	DOREPLIFETIME(UFirearm, AmmoReloadTime);
	DOREPLIFETIME(UFirearm, ContainedAmmo);
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

	return OwnerPlayerPawn->GetClientCameraRotation().RotateVector(FirearmDefinition->GetShootingOffset());
}

bool UFirearm::Fire()
{
	if (NextTimeToFire > GetWorld()->GetTimeSeconds())
		return false;

	const FVector shootingDirection = OwnerPlayerPawn->GetPlayerCombatComponent()->GetAimAdjustDirection(GetShootingOffset());
	const bool bDidFire = FirearmDefinition->OnFire(this, shootingDirection);

	if (FireRate > 0)
		NextTimeToFire = GetWorld()->GetTimeSeconds() + 1.0f / FireRate;

	if (bDidFire)
		UGameplayStatics::PlaySoundAtLocation(OwnerPlayerPawn.Get(), FirearmDefinition->GetShotSound(), OwnerPlayerPawn->GetActorLocation());
	
	return bDidFire;
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
	FAmmoContainer* container = FAmmoContainer::FindAmmoContainer(ContainedAmmo, AmmoDefinition);

	if (!container)
		return false;

	if (container->AmmoAmount < AmmoAmount)
		return false;

	return true;
}

bool UFirearm::ConsumeAmmo(UAmmoDefinition* AmmoDefinition, int AmmoAmount)
{
	if (CanConsumeAmmo(AmmoDefinition, AmmoAmount))
	{
		FAmmoContainer::FindAmmoContainer(ContainedAmmo, AmmoDefinition)->AmmoAmount -= AmmoAmount;
		return true;
	}

	return false;
}

bool UFirearm::CanReloadAmmo()
{
	for (FAmmoContainer& container : ContainedAmmo) 
	{
		const bool notFull = container.AmmoAmount < AmmoCapacity;
		const bool haveFreeAmmo = OwnerPlayerPawn->GetPlayerCombatComponent()->GetAmmoAmount(container.AmmoDefinition) > 0;
		if (notFull && haveFreeAmmo)
			return true;
	}

	return false;
}

void UFirearm::ReloadAmmo()
{
	for (FAmmoContainer& container : ContainedAmmo)
	{
		const int availableAmmo = OwnerPlayerPawn->GetPlayerCombatComponent()->GetAmmoAmount(container.AmmoDefinition);
		const int needAmmo = AmmoCapacity - container.AmmoAmount;
		const int ammoToConsume = FMath::Min(availableAmmo, needAmmo);

		if (!OwnerPlayerPawn->GetPlayerCombatComponent()->ConsumeAmmo(container.AmmoDefinition, ammoToConsume))
			continue;
		
		container.AmmoAmount += ammoToConsume;

		UGameplayStatics::PlaySoundAtLocation(OwnerPlayerPawn.Get(), FirearmDefinition->GetReloadSound(), OwnerPlayerPawn->GetActorLocation());
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