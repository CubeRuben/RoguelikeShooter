#pragma once

#include "CoreMinimal.h"
#include "FirearmBaseFire.h"
#include "FirearmHitscanFire.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API UFirearmHitscanFire : public UFirearmBaseFire
{
	GENERATED_BODY()

public:

	UFirearmHitscanFire();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HitMaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScatterAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraSystem* TraceParticleSystem;

public: 

	void OnFire(class UFirearm* Firearm, FVector ShootingDirection) override;
	
	void SpawnVisual(class UFirearm* Firearm, FVector StartLocation, FVector EndLocation);
};
