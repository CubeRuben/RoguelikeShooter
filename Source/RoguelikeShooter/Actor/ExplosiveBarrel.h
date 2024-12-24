#pragma once

#include "CoreMinimal.h"
#include "DamageableProp.h"
#include "ExplosiveBarrel.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API AExplosiveBarrel : public ADamageableProp
{
	GENERATED_BODY()

public:

	AExplosiveBarrel();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinTimeForExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxTimeForExplosion;

	FTimerHandle ExplosionTimerHandle;

	void OnDestroy() override;

	void Explode();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetOnFire();

public:

	UFUNCTION(NetMulticast, Reliable)
	void SetOnFire_MulticastRPC();
	void SetOnFire_MulticastRPC_Implementation();
};
