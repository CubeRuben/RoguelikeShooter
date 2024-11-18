#include "PlayerCombatComponent.h"

#include "../PlayerPawn.h"
#include "../../CombatSystem/Firearm.h"
#include "../../CombatSystem/AmmoDefinition.h"

#include <Camera/CameraComponent.h>

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

	HandleInput();
}

void UPlayerCombatComponent::HandleInput()
{
	FPlayerInput& playerInput = PlayerPawn->GetPlayerInput();
	UFirearm* currentFirearm = GetCurrentFirearm();

	if (!currentFirearm)
	{
		return;
	}

	if (playerInput.MouseWheel > 0.0f)
	{
		const int amountOfHeldFirearms = HeldFirearms.Num();
		SetCurrentFirearm((CurrentFirearmIndex - 1 + amountOfHeldFirearms) % amountOfHeldFirearms);
	}
	else if (playerInput.MouseWheel < 0.0f)
	{
		const int amountOfHeldFirearms = HeldFirearms.Num();
		SetCurrentFirearm((CurrentFirearmIndex + 1) % amountOfHeldFirearms);
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

FVector UPlayerCombatComponent::GetAimAdjustDirection(FVector Offset)
{
	const UCameraComponent* cameraComponent = PlayerPawn->GetCameraComponent();
	const FVector startPosition = cameraComponent->GetComponentLocation();
	const FVector endPosition = startPosition + cameraComponent->GetForwardVector() * 2000.0f;

	FCollisionQueryParams params;
	params.AddIgnoredActor(PlayerPawn);

	FHitResult hit;
	if (!GetWorld()->LineTraceSingleByProfile(hit, startPosition, endPosition, "BlockAll", params))
		return cameraComponent->GetForwardVector();

	return (hit.Location - (startPosition + Offset)).GetSafeNormal();
}

void UPlayerCombatComponent::AddFirearm(UFirearm* NewFirearm)
{
	if (!NewFirearm)
		return;

	const int index = HeldFirearms.Add(NewFirearm);
	NewFirearm->SetOwnerPlayerPawn(PlayerPawn);
	SetCurrentFirearm(index);
}

void UPlayerCombatComponent::SetCurrentFirearm(int Index)
{
	if (!HeldFirearms.IsValidIndex(CurrentFirearmIndex))
		return;

	CurrentFirearmIndex = Index;
}

bool UPlayerCombatComponent::AddAmmo(UAmmoDefinition* AmmoDefinition, int AmmoAmount)
{
	if (!AmmoDefinition)
		return false;

	int* ammoAmount = ContainedAmmo.Find(AmmoDefinition);

	if (ammoAmount) 
	{
		if (*ammoAmount >= AmmoDefinition->GetMaxAmount()) 
			return false;
		
		*ammoAmount = FMath::Min(AmmoDefinition->GetMaxAmount(), *ammoAmount + AmmoAmount);
		return true;
	}

	ContainedAmmo.Add(AmmoDefinition, FMath::Min(AmmoDefinition->GetMaxAmount(), AmmoAmount));
	return true;
}

bool UPlayerCombatComponent::CanConsumeAmmo(UAmmoDefinition* AmmoDefinition, int AmmoAmount)
{
	int* ammoAmount = ContainedAmmo.Find(AmmoDefinition);

	if (!ammoAmount)
		return false;

	if (*ammoAmount < AmmoAmount)
		return false;

	return true;
}

bool UPlayerCombatComponent::ConsumeAmmo(UAmmoDefinition* AmmoDefinition, int AmmoAmount)
{
	if (CanConsumeAmmo(AmmoDefinition, AmmoAmount))
	{
		*ContainedAmmo.Find(AmmoDefinition) -= AmmoAmount;
		return true;
	}
	
	return false;
}
