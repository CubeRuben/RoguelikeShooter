#pragma once

#include "CoreMinimal.h"
#include "../ExtendedEngine/NetworkObject.h"
#include "AmmoContainer.h"
#include "Firearm.generated.h"

UCLASS(EditInlineNew)
class ROGUELIKESHOOTER_API UFirearm : public UNetworkObject
{
	GENERATED_BODY()
	
public:

	UFirearm();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TWeakObjectPtr<class APlayerPawn> OwnerPlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	class UFirearmDefinition* FirearmDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	int AmmoCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	float AmmoReloadTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<FAmmoContainer> ContainedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NextTimeToFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsAutoFireMode;

	void InitFirearm();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	void SetOwnerPlayerPawn(class APlayerPawn* NewOwnerPlayerPawn);
	class APlayerPawn* GetOwnerPlayerPawn() const;

	void SetFirearmDefinition(class UFirearmDefinition* NewFirearmDefinition);
	class UFirearmDefinition* GetFirearmDefinition() const { return FirearmDefinition; }

	float GetDamage() const { return Damage; }
	float GetFireRate() const { return FireRate; }
	float GetAmmoReloadTime() const { return AmmoReloadTime; }
	FVector GetShootingOffset() const;

	bool Fire();
	void AlternativeAction();

	bool GetIsAutoFireMode() const { return bIsAutoFireMode; }
	void SetIsAutoFireMode(bool newMode) { bIsAutoFireMode = newMode; }
	void SwitchFireMode() { bIsAutoFireMode = !bIsAutoFireMode; }

	bool CanConsumeAmmo(class UAmmoDefinition* AmmoDefinition, int AmmoAmount);
	bool ConsumeAmmo(class UAmmoDefinition* AmmoDefinition, int AmmoAmount);

	bool CanReloadAmmo();
	void ReloadAmmo();

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif
};
