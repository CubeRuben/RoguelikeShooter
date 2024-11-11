#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class ROGUELIKESHOOTER_API IInteractable
{
	GENERATED_BODY()

public:

	virtual void Interact(class APlayerPawn* Pawn) = 0;

	virtual void StartHovering() {}
	virtual void StopHovering() {}
	
};
