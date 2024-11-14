#pragma once

#include "CoreMinimal.h"
#include "../ExtendedEngine/NetworkObject.h"
#include "Firearm.generated.h"

UCLASS(EditInlineNew)
class ROGUELIKESHOOTER_API UFirearm : public UNetworkObject
{
	GENERATED_BODY()
	
public:

	UFirearm();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class APlayerPawn> OwnerPlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFirearmDefinition* FirearmDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsAutoFireMode;

	void InitFirearm();

public:

	void SetOwnerPlayerPawn(class APlayerPawn* NewOwnerPlayerPawn);
	class APlayerPawn* GetOwnerPlayerPawn() const;

	void SetFirearmDefinition(class UFirearmDefinition* NewFirearmDefinition);
	class UFirearmDefinition* GetFirearmDefinition() const { return FirearmDefinition; }

	bool Fire();
	void AlternativeAction();

	bool GetIsAutoFireMode() const { return bIsAutoFireMode; }
	void SetIsAutoFireMode(bool newMode) { bIsAutoFireMode = newMode; }
	void SwitchFireMode() { bIsAutoFireMode = !bIsAutoFireMode; }
};
