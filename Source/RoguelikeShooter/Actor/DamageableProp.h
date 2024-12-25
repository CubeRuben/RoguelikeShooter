#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../CombatSystem/Damageable.h"
#include "DamageableProp.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API ADamageableProp : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	

	ADamageableProp();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthPoints;

	virtual void OnDestroy();

public:	

	virtual void ApplyDamage(float DamageAmount, FDamageParams* DamageParams = nullptr) override;
	void ApplyImpulse(FVector Impulse) override;
	FVector GetLocation() override { return GetActorLocation(); }


};
