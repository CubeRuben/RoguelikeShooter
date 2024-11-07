#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHealthComponent.generated.h"



UCLASS()
class ROGUELIKESHOOTER_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerHealthComponent();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthPoints;

	virtual void BeginPlay() override;

public:	

	void SetHealthPoints(float NewHealthPoints);
	float GetHealthPoints() const;
	
	void ApplyDamage(float DamageAmount);
	void Heal(float HealAmount);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
