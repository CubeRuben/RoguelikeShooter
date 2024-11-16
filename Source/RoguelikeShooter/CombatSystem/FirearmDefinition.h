#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FirearmDefinition.generated.h"

USTRUCT()
struct FRandomFloatProperty
{
	GENERATED_BODY()

public:

	FRandomFloatProperty();

protected:

	UPROPERTY(EditAnywhere)
	float MinValue;

	UPROPERTY(EditAnywhere)
	float MaxValue;

public:

	float GetRandomValue() const;
	float GetMinValue() const { return MinValue; }
	float GetMaxValue() const { return MaxValue; }
};

UCLASS()
class ROGUELIKESHOOTER_API UFirearmDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public: 

	UFirearmDefinition() {}

protected:

	UPROPERTY(EditAnywhere)
	FRandomFloatProperty Damage;

	UPROPERTY(EditAnywhere)
	FRandomFloatProperty FireRate;

	UPROPERTY(EditAnywhere)
	FVector ShootingOffset;	

	UPROPERTY(EditAnywhere)
	class UStaticMesh* FirearmMesh;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<class UFirearmBaseFire*> OnFireBehaviour;

public:

	float GetDamage() const { return Damage.GetRandomValue(); }
	float GetFireRate() const { return FireRate.GetRandomValue(); }
	FVector GetShootingOffset() const { return ShootingOffset; }
	
	class UStaticMesh* GetFirearmMesh() const { return FirearmMesh; }

	void OnFire(class UFirearm* Firearm, FVector ShootingDirection);
};
