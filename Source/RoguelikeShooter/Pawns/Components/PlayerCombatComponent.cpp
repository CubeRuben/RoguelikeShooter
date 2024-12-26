#include "PlayerCombatComponent.h"

#include "../PlayerPawn.h"
#include "../../CombatSystem/Firearm.h"
#include "../../CombatSystem/AmmoDefinition.h"
#include "../../CombatSystem/Actors/FirearmPickup.h"

#include <Camera/CameraComponent.h>
#include <Net/UnrealNetwork.h>
#include <Engine/ActorChannel.h>

UPlayerCombatComponent::UPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	CurrentFirearmIndex = 0;
	CurrentFirearmIndexReplicated = 0;
}

void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerPawn>(GetOwner());

	if (!PlayerPawn)
		DestroyComponent();	
}

void UPlayerCombatComponent::DropAllFirearms()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (UFirearm* firearm : HeldFirearms) 
	{
		AFirearmPickup* firearmPickup = GetWorld()->SpawnActor<AFirearmPickup>(AFirearmPickup::StaticClass(), PlayerPawn->GetClientLocation(), FRotator::ZeroRotator, spawnParams);

		if (!firearmPickup)
			return;

		firearmPickup->SetFirearm(firearm);
	}

	HeldFirearms.Empty();
}

void UPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleInput();

	UpdateReloading(DeltaTime);
}

void UPlayerCombatComponent::HandleInput()
{
	FPlayerInput& playerInput = PlayerPawn->GetPlayerInput();
	UFirearm* currentFirearm = GetCurrentFirearm();

	if (!currentFirearm)
	{
		return;
	}

	if (playerInput.MouseWheel != 0.0f) 
	{
		const int amountOfHeldFirearms = HeldFirearms.Num();

		if (playerInput.MouseWheel > 0.0f)
		{
			SetCurrentFirearm((CurrentFirearmIndex - 1 + amountOfHeldFirearms) % amountOfHeldFirearms);
		}
		else if (playerInput.MouseWheel < 0.0f)
		{
			SetCurrentFirearm((CurrentFirearmIndex + 1) % amountOfHeldFirearms);
		}

		if (bIsReloading)
			StopReloading(true);

		return;
	}

	if (bIsReloading)
		return;

	if (playerInput.bDropWeapon) 
	{
		playerInput.bDropWeapon = false;

		if (PlayerPawn->HasAuthority())
			DropFirearm(currentFirearm);
		else
			DropFirearm_ServerRPC(currentFirearm);

		return;
	}

	if (playerInput.bFireWeapon) 
	{
		if (!currentFirearm->GetIsAutoFireMode())
			playerInput.bFireWeapon = false;

		const bool bDidFire = currentFirearm->Fire();

		if (bDidFire)
			OnFirearmFire.Broadcast();

		if (!PlayerPawn->HasAuthority())
		{
			Fire_ServerRPC();
		}
		else 
		{
			Fire_MulticastRPC();
		}
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

		if (!PlayerPawn->HasAuthority())
			SetFireMode_ServerRPC(currentFirearm->GetIsAutoFireMode());
	}

	if (playerInput.bReload) 
	{
		playerInput.bReload = false;

		if (currentFirearm->CanReloadAmmo())
			StartReloading();
	}
}

void UPlayerCombatComponent::UpdateReloading(float DeltaTime)
{
	if (!bIsReloading)
		return;

	ReloadingTimer -= DeltaTime;

	if (ReloadingTimer > 0.0f)
		return;
	
	GetCurrentFirearm()->ReloadAmmo();

	StopReloading();
}

void UPlayerCombatComponent::StartReloading()
{
	UFirearm* currentFirearm = GetCurrentFirearm();

	if (!currentFirearm)
		return;

	ReloadingTimer = currentFirearm->GetAmmoReloadTime();
	bIsReloading = true;
	OnReloadStart.Broadcast();

	if (PlayerPawn->IsLocallyControlled() && !PlayerPawn->HasAuthority())
		ReloadAmmo_ServerRPC();
}

void UPlayerCombatComponent::StopReloading(bool bForce)
{
	ReloadingTimer = 0.0f;
	bIsReloading = false;
	OnReloadStop.Broadcast(bForce);
}

void UPlayerCombatComponent::DropFirearm(UFirearm* Firearm)
{
	if (!Firearm) 
		return;

	HeldFirearms.Contains(Firearm);

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AFirearmPickup* firearmPickup = GetWorld()->SpawnActor<AFirearmPickup>(AFirearmPickup::StaticClass(), PlayerPawn->GetClientLocation(), FRotator::ZeroRotator, spawnParams);

	if (!firearmPickup)
		return;

	firearmPickup->SetFirearm(Firearm);
	HeldFirearms.Remove(Firearm);

	const int num = HeldFirearms.Num();
	if (num == 0) 
	{
		SetCurrentFirearm(0);
	}
	else 
	{
		SetCurrentFirearm(CurrentFirearmIndex % num);
	}
}

void UPlayerCombatComponent::OnRep_CurrentFirearmIndexReplicated()
{
	if (!PlayerPawn->HasAuthority()  && !PlayerPawn->IsLocallyControlled())
		SetCurrentFirearm(CurrentFirearmIndexReplicated);
}

void UPlayerCombatComponent::OnRep_HeldFirearms()
{
	SetCurrentFirearm(CurrentFirearmIndexReplicated);
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

	if (HeldFirearms.Contains(NewFirearm))
		return;

	const int index = HeldFirearms.Add(NewFirearm);
	NewFirearm->SetOwnerPlayerPawn(PlayerPawn);
	SetCurrentFirearm(index);
}

void UPlayerCombatComponent::SetCurrentFirearm(int Index)
{
	if (!HeldFirearms.IsValidIndex(Index)) 
	{
		OnCurrentFirearmChange.Broadcast(nullptr, Index);
		return;
	}

	CurrentFirearmIndex = Index;

	OnCurrentFirearmChange.Broadcast(GetCurrentFirearm(), Index);

	if (PlayerPawn->HasAuthority())
		CurrentFirearmIndexReplicated = CurrentFirearmIndex;

	if (!PlayerPawn->HasAuthority() && PlayerPawn->IsLocallyControlled())
		SetCurrentFirearm_ServerRPC(CurrentFirearmIndex);
}

float UPlayerCombatComponent::GetReloadTimerRatio()
{
	UFirearm* firearm = GetCurrentFirearm();

	if (firearm)
		return 1.0f - ReloadingTimer / firearm->GetAmmoReloadTime();

	return 1.0f;
}

bool UPlayerCombatComponent::AddAmmo(UAmmoDefinition* AmmoDefinition, int AmmoAmount)
{
	if (!AmmoDefinition)
		return false;

	FAmmoContainer* container = FAmmoContainer::FindAmmoContainer(ContainedAmmo, AmmoDefinition);

	if (container)
	{
		if (container->AmmoAmount >= AmmoDefinition->GetMaxAmount())
			return false;
		
		container->AmmoAmount = FMath::Min(AmmoDefinition->GetMaxAmount(), container->AmmoAmount + AmmoAmount);
		return true;
	}

	ContainedAmmo.Add(FAmmoContainer(AmmoDefinition, FMath::Min(AmmoDefinition->GetMaxAmount(), AmmoAmount)));
	return true;
}

int UPlayerCombatComponent::GetAmmoAmount(UAmmoDefinition* AmmoDefinition) const
{
	const FAmmoContainer* container = FAmmoContainer::FindAmmoContainer(ContainedAmmo, AmmoDefinition);

	if (container)
		return container->AmmoAmount;

	return 0;
}

bool UPlayerCombatComponent::CanConsumeAmmo(UAmmoDefinition* AmmoDefinition, int AmmoAmount)
{
	FAmmoContainer* container = FAmmoContainer::FindAmmoContainer(ContainedAmmo, AmmoDefinition);

	if (!container)
		return false;

	if (container->AmmoAmount < AmmoAmount)
		return false;

	return true;
}

bool UPlayerCombatComponent::ConsumeAmmo(UAmmoDefinition* AmmoDefinition, int AmmoAmount)
{
	if (CanConsumeAmmo(AmmoDefinition, AmmoAmount))
	{
		FAmmoContainer::FindAmmoContainer(ContainedAmmo, AmmoDefinition)->AmmoAmount -= AmmoAmount;
		return true;
	}

	return false;
}

void UPlayerCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerCombatComponent, CurrentFirearmIndexReplicated);
	DOREPLIFETIME(UPlayerCombatComponent, HeldFirearms);
	DOREPLIFETIME(UPlayerCombatComponent, ContainedAmmo);
}

bool UPlayerCombatComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool wroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (auto& firearm : HeldFirearms)
	{
		wroteSomething |= firearm->ReplicateSubobjects(Channel, Bunch, RepFlags);
		wroteSomething |= Channel->ReplicateSubobject(firearm, *Bunch, *RepFlags);
	}

	return wroteSomething;
}

void UPlayerCombatComponent::Fire_ServerRPC_Implementation()
{
	UFirearm* currentFirearm = GetCurrentFirearm();

	if (!currentFirearm)
		return;

	if (currentFirearm->Fire())
		Fire_MulticastRPC();
}

void UPlayerCombatComponent::Fire_MulticastRPC_Implementation()
{
	if (PlayerPawn->HasAuthority() || PlayerPawn->IsLocallyControlled())
		return;

	UFirearm* currentFirearm = GetCurrentFirearm();

	if (!currentFirearm)
		return;

	currentFirearm->Fire();
}

void UPlayerCombatComponent::SetCurrentFirearm_ServerRPC_Implementation(int Index)
{
	SetCurrentFirearm(Index);
}

void UPlayerCombatComponent::ReloadAmmo_ServerRPC_Implementation()
{
	StartReloading();
}

void UPlayerCombatComponent::SetFireMode_ServerRPC_Implementation(bool bFireMode)
{
	UFirearm* firearm = GetCurrentFirearm();

	if (firearm)
		firearm->SetIsAutoFireMode(bFireMode);
}

void UPlayerCombatComponent::DropFirearm_ServerRPC_Implementation(UFirearm* Firearm)
{
	DropFirearm(Firearm);
}