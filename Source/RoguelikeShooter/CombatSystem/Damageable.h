#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

USTRUCT(BlueprintType)
struct FDamageParams
{
	GENERATED_BODY()

public:

	FDamageParams() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* DamageSource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector HitLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector HitDirection;

};

UINTERFACE(MinimalAPI, BlueprintType)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class ROGUELIKESHOOTER_API IDamageable
{
	GENERATED_BODY()

public:

	virtual void ApplyDamage(float DamageAmount, FDamageParams* = nullptr) {}
	virtual void ApplyImpulse(FVector Impulse) {}
};
