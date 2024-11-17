#pragma once

#include "CoreMinimal.h"
#include "FirearmBaseFire.h"
#include "FirearmProjectileFire.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API UFirearmProjectileFire : public UFirearmBaseFire
{
	GENERATED_BODY()
	
public:

	UFirearmProjectileFire();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ABaseProjectile> ProjectileClass;

public:

	void OnFire(class UFirearm* Firearm, FVector ShootingDirection) override;
};
