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

USTRUCT()
struct FRandomIntProperty
{
	GENERATED_BODY()

public:

	FRandomIntProperty();

protected:

	UPROPERTY(EditAnywhere)
	int MinValue;

	UPROPERTY(EditAnywhere)
	int MaxValue;

public:

	int GetRandomValue() const;
	int GetMinValue() const { return MinValue; }
	int GetMaxValue() const { return MaxValue; }
};

USTRUCT(BlueprintType)
struct FFirearmUIData
{
	GENERATED_BODY()

public:

	FFirearmUIData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* FirearmShape;
};

UCLASS(BlueprintType)
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
	FRandomIntProperty AmmoCapacity;

	UPROPERTY(EditAnywhere)
	FRandomFloatProperty AmmoReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector ShootingOffset;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* FirearmMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FFirearmUIData FirearmUIData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* ShotSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<class UFirearmBaseFire*> OnFireBehaviour;

public:

	float GetDamage() const { return Damage.GetRandomValue(); }
	float GetFireRate() const { return FireRate.GetRandomValue(); }
	int GetAmmoCapacity() const { return AmmoCapacity.GetRandomValue(); }
	float GetAmmoReloadTime() const {return AmmoReloadTime.GetRandomValue(); }
	FVector GetShootingOffset() const { return ShootingOffset; }

	USoundBase* GetShotSound() const { return ShotSound; };
	USoundBase* GetReloadSound() const { return ReloadSound; };
	
	class UStaticMesh* GetFirearmMesh() const { return FirearmMesh; }

	void GetRequiredAmmoTypes(TSet<class UAmmoDefinition*>& RequiredAmmo) const;

	bool OnFire(class UFirearm* Firearm, FVector ShootingDirection);
};
