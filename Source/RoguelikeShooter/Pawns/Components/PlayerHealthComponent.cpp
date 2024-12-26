#include "PlayerHealthComponent.h"

#include "PlayerCombatComponent.h"
#include "../PlayerPawn.h"

#include <Net/UnrealNetwork.h>
#include <GameFramework/GameMode.h>

UPlayerHealthComponent::UPlayerHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	HealthPoints = 100.0f;
}

void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerHealthComponent, HealthPoints);
}

void UPlayerHealthComponent::OnDeath()
{
	if (!GetOwner()->HasAuthority())
		return;

	APlayerPawn* playerPawn = Cast<APlayerPawn>(GetOwner());

	if (!playerPawn)
		return;

	APlayerController* playerController = Cast<APlayerController>(playerPawn->Controller);

	if (!playerController)
		return;

	playerController->UnPossess();
	GetWorld()->GetAuthGameMode()->RestartPlayer(playerController);

	playerPawn->GetPlayerCombatComponent()->DropAllFirearms();

	GetOwner()->Destroy();
}

void UPlayerHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerHealthComponent::SetHealthPoints(float NewHealthPoints)
{
	HealthPoints = FMath::Clamp(NewHealthPoints, 0.0f, GetMaxHealthPoints());
}

void UPlayerHealthComponent::ApplyDamage(float DamageAmount, AActor* DamageSource)
{
	if (!GetOwner()->HasAuthority())
		return;

	if (DamageAmount < 0.0f)
		return;

	HealthPoints -= DamageAmount;

	if (HealthPoints <= 0)
		OnDeath();

	OnDamageTaken.Broadcast(DamageAmount, DamageSource);
}

void UPlayerHealthComponent::Heal(float HealAmount)
{
	if (HealAmount < 0.0f)
		return;

	HealthPoints = FMath::Min(HealthPoints + HealAmount, GetMaxHealthPoints());
}

float UPlayerHealthComponent::GetMaxHealthPoints() const
{
	return 100.0f;
}

