#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHealthComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)	
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTakenDelegate, float, DamageTaken, AActor*, DamageSource);

UCLASS()
class ROGUELIKESHOOTER_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerHealthComponent();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float HealthPoints;
	
	UPROPERTY(BlueprintAssignable)
	FOnDamageTakenDelegate OnDamageTaken;

	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	void SetHealthPoints(float NewHealthPoints);
	float GetHealthPoints() const { return HealthPoints; };

	void ApplyDamage(float DamageAmount, AActor* DamageSource = nullptr);
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealthPoints() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealthPointsRatio() const { return HealthPoints / GetMaxHealthPoints(); };

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
