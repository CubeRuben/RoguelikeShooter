#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FirearmBaseFire.generated.h"

UCLASS(EditInlineNew, Abstract)
class ROGUELIKESHOOTER_API UFirearmBaseFire : public UObject
{
	GENERATED_BODY()
	
public:

	UFirearmBaseFire();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAmmoDefinition* AmmoDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int AmmoConsumption;

public:

	virtual void OnFire(class UFirearm* Firearm, FVector ShootingDirection);

	class UAmmoDefinition* GetAmmoDefinition() const { return AmmoDefinition; }
	int GetAmmoConsumption() const { return AmmoConsumption; }
};
