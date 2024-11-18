#include "Firearm.h"

#include "FirearmDefinition.h"
#include "../Pawns/PlayerPawn.h"
#include "../Pawns/Components/PlayerCombatComponent.h"

#include <Camera/CameraComponent.h>

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

void UFirearm::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(UFirearm, FirearmDefinition))
	{
		InitFirearm();
	}
}