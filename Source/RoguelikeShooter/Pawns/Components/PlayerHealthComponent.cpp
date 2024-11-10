#include "PlayerHealthComponent.h"

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
	if (DamageAmount < 0.0f)
		return;

	HealthPoints -= DamageAmount;

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

