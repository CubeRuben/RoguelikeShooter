#include "PlayerCombatComponent.h"

#include "../PlayerPawn.h"
#include "../../CombatSystem/Firearm.h"

UPlayerCombatComponent::UPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentFirearmIndex = 0;
}

void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerPawn>(GetOwner());

	if (!PlayerPawn)
		DestroyComponent();	
}

void UPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerCombatComponent::HandleInput()
{
	FPlayerInput& playerInput = PlayerPawn->GetPlayerInput();
	UFirearm* currentFirearm = GetCurrentFirearm();

	if (!currentFirearm)
	{
		return;
	}

	if (playerInput.bFireWeapon) 
	{
		if (!currentFirearm->GetIsAutoFireMode())
			playerInput.bFireWeapon = false;

		currentFirearm->Fire();
	}

	if (playerInput.bAlternativeWeaponAction) 
	{
		playerInput.bAlternativeWeaponAction = false;

		currentFirearm->AlternativeAction();
	}

	if (playerInput.bSwitchFireMode) 
	{
		playerInput.bSwitchFireMode = false;

		currentFirearm->SwitchFireMode();
	}
}

UFirearm* UPlayerCombatComponent::GetCurrentFirearm()
{
	if (HeldFirearms.IsValidIndex(CurrentFirearmIndex))
		return HeldFirearms[CurrentFirearmIndex];

	return nullptr;
}
