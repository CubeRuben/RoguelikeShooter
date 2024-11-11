#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractablePickup.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UInteractablePickup : public UInterface
{
	GENERATED_BODY()
};

class ROGUELIKESHOOTER_API IInteractablePickup
{
	GENERATED_BODY()

public:

	virtual void Pickup(class APlayerPawn* Pawn) = 0;

};
