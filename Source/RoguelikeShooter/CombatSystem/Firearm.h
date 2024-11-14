#pragma once

#include "CoreMinimal.h"
#include "../ExtendedEngine/NetworkObject.h"
#include "Firearm.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API UFirearm : public UNetworkObject
{
	GENERATED_BODY()
	
public:

	UFirearm();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class APlayerPawn> OwnerPlayerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFirearmDefinition* FirearmDefinition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAutoFireMode;

public:

	void Fire();
	void AlternativeAction();

	bool GetIsAutoFireMode() const { return bIsAutoFireMode; }
	void SetIsAutoFireMode(bool newMode) { bIsAutoFireMode = newMode; }
	void SwitchFireMode() { bIsAutoFireMode = !bIsAutoFireMode; }
	

};
